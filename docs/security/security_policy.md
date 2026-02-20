# GSM4 Firmware Rewrite, Security Policy (Draft)

This document defines security requirements and implementation rules to be followed throughout the GSM4 firmware rewrite (ESP-IDF). It is written to be practical: each section includes mandatory rules, design intent, and verification checks.

Status: Draft. Owner: Engineering. Applies to: all firmware components, build configurations, manufacturing tooling, and field update processes.

---

## 1. Security objectives

1. Protect confidentiality and integrity of device secrets (private keys, credentials, provisioning data).
2. Ensure integrity and authenticity of firmware images and updates.
3. Prevent unauthorised firmware execution and downgrade attacks.
4. Minimise attack surface in production builds.
5. Enable traceable, auditable manufacturing provisioning and secure update workflows.

---

## 2. Definitions

- **Secret**: Any credential that would grant access or allow impersonation, for example private keys, symmetric keys, tokens, passwords, PSKs, database credentials, API keys.
- **Provisioning**: The manufacturing or secure process by which secrets and identities are injected into the device.
- **Production build**: A build intended for field deployment.
- **Debug build**: A development build that may allow additional observability, but must not weaken production invariants in a way that can ship.

---

## 3. Mandatory rules (project wide)

### 3.1 No secrets in source control
- Secrets must never be committed to Git (including in examples or test data).
- Any placeholder sample values must be obviously non-functional, for example `EXAMPLE_ONLY_DO_NOT_USE`.

Verification:
- Pre-commit hooks or CI secret scanning should be enabled when available.
- Code review must explicitly check for secrets in config files, logs, and test fixtures.

### 3.2 No secrets in logs
- Logs must never include private keys, raw certificates, session keys, tokens, or full credential blobs.
- Debug commands must not print secret material even in debug builds.
- If diagnostics require proving a secret exists, log only a fingerprint (SHA-256) or key ID, never the secret itself.

Verification:
- Unit tests should include a “log redaction” test for any diagnostic dump features.
- Code review checklist includes “no secret leakage” as a required tick.

### 3.3 Secure defaults and config gating
- Any debug feature that increases attack surface must be behind a build flag and default to off.
- Production builds must disable:
  - debug shells
  - AT pass-through consoles
  - unsecured local management endpoints
  - verbose diagnostic dumps

Verification:
- `sdkconfig.defaults` for production must explicitly set these flags off.
- CI should build a “production config” target to catch regressions.

### 3.4 Cryptography and protocol choices
- Prefer modern, well maintained primitives exposed by ESP-IDF and mbedTLS.
- Do not implement custom cryptography.
- Do not weaken TLS settings to “make it work” without an approved exception.

Verification:
- TLS configuration is documented and reviewed.
- Known weak ciphers and protocol versions are disabled in production.

---

## 4. Secure Boot

### 4.1 Requirement
- Secure Boot must be enabled for production firmware.

Design intent:
- Ensure only signed firmware images can boot, preventing unauthorised firmware execution.

### 4.2 Secure Boot policy
- Use ESP32 Secure Boot v2 where supported by the chosen module and ESP-IDF version.
- Secure Boot private signing keys must never be on developer laptops without explicit approval.
- Signing keys must be stored in a controlled environment (HSM or equivalent secure key store).

### 4.3 Anti-rollback
- Enable anti-rollback to prevent downgrades to vulnerable firmware versions.
- Firmware versioning must monotonically increase for production releases.

Verification:
- Manufacturing procedure verifies eFuses are correctly programmed for Secure Boot.
- Release process verifies images are signed and anti-rollback version is correct.

---

## 5. Flash Encryption

### 5.1 Requirement
- Flash Encryption must be enabled for production firmware.

Design intent:
- Protect secrets at rest and reduce risk of physical extraction of credentials.

### 5.2 Policy
- Use ESP-IDF supported Flash Encryption modes appropriate to the product lifecycle.
- Flash encryption keys must be generated on device and locked using eFuses as per ESP-IDF guidance.

Verification:
- Manufacturing procedure verifies flash encryption is enabled and relevant eFuses are set.
- Attempted plaintext readout of flash content should not reveal secrets.

---

## 6. NVS and data at rest

### 6.1 Requirement
- NVS must be used for persistent configuration and provisioning metadata.
- NVS Encryption must be enabled for production if secrets are stored in NVS.

### 6.2 Policy
- Separate namespaces for:
  - system config
  - calibration data
  - provisioning and identity
  - application state
- Never store raw private keys in plaintext NVS.
- Store private keys using ESP-IDF secure storage mechanisms where possible (NVS encryption + flash encryption + restricted access patterns).

Verification:
- Unit tests validate that secret fields are never stored via insecure code paths.
- Production build enables NVS encryption where secrets exist.

---

## 7. Storage and handling of secrets

### 7.1 What counts as a secret
- Private keys (device identity keys, signing keys, session keys)
- Symmetric keys, PSKs
- Passwords and tokens
- Provisioning blobs that can be used to derive any of the above

### 7.2 Handling rules
- Secrets should be held in RAM only as long as needed.
- Zeroise memory buffers after use where practical.
- Do not duplicate secret buffers unnecessarily.
- Do not include secrets in crash dumps.

Verification:
- Code review checks for secure buffer handling and minimal lifetime.
- Where feasible, add unit tests for “zeroise after use” behaviour for key handling modules.

---

## 8. X.509 certificates

### 8.1 Requirement
- Device authentication must use X.509 certificates for mutual authentication where applicable.

### 8.2 Policy
- The device must have a unique device identity:
  - device private key
  - device certificate signed by the organisation CA or intermediate CA
- Trust anchors (CA certificates) must be pinned and updated only through authenticated firmware update mechanisms.
- Certificates must have defined lifetimes, and certificate rotation must be supported.

Verification:
- The TLS stack validates the server certificate chain and hostname.
- Mutual TLS is validated in integration testing.

---

## 9. TLS v1.3

### 9.1 Requirement
- Prefer TLS v1.3 for all network connections where supported by the modem and firmware stack.
- If TLS v1.3 is not technically possible for a particular connection path, document the exception and use the strongest available alternative (for example TLS v1.2 with restricted ciphers).

### 9.2 Policy
- Disable legacy protocol versions (SSL, TLS 1.0, TLS 1.1).
- Restrict cipher suites to modern AEAD suites (as supported by the chosen TLS stack).

Verification:
- Integration testing confirms negotiated protocol version and cipher suite.
- Security review approves any exception.

---

## 10. Key provisioning, use, and lifecycle

### 10.1 Provisioning requirements
- Each device must be provisioned with unique keys and identity.
- Provisioning must be auditable:
  - device identifier
  - certificate serial number
  - provisioning timestamp
  - operator or station ID
- Provisioning must not expose private keys to uncontrolled environments.

### 10.2 Recommended approach
- Generate device private key on device when possible.
- Use CSR flow:
  1. Device generates key pair and CSR.
  2. Manufacturing station submits CSR to signing service.
  3. Device receives signed certificate and stores it securely.
- If keys cannot be generated on device, use a secure injection process and ensure keys are never stored in plaintext outside controlled systems.

### 10.3 Key rotation and revocation
- Support certificate rotation through authenticated update or provisioning workflows.
- Plan for revocation:
  - server side CRL or OCSP as applicable
  - local trust anchor update capability via signed firmware

Verification:
- Manufacturing test verifies identity and certificate validity.
- Field update procedure includes a rotation path if required.

---

## 11. Firmware update security (OTA)

### 11.1 Requirement
- OTA images must be authenticated and integrity protected.
- Prefer signed updates, verified before activation.

### 11.2 Policy
- Use ESP-IDF OTA with image verification aligned to Secure Boot.
- Enforce anti-rollback versioning on production devices.
- Support dual image / fallback behaviour to recover from failed updates.

Verification:
- OTA test confirms failure cases revert to known good image.
- Tampered image is rejected.

---

## 12. Debug and manufacturing modes

### 12.1 Debug builds
- Debug builds may include extra logs and tools, but:
  - must never include production keys
  - must never bypass signature checks in a way that can ship

### 12.2 Manufacturing mode
- Manufacturing mode must be controlled:
  - only enabled on the line
  - disabled or locked out for production devices post provisioning
- Any manufacturing commands must be authenticated or only available behind physical presence controls where feasible.

Verification:
- Production build config disables manufacturing-only features.
- A test ensures “manufacturing mode” is not reachable in production.

---

## 13. Threat surface reduction

- Minimise open ports and services.
- Avoid listening sockets unless required.
- Restrict all local interfaces:
  - serial debug
  - AT command passthrough
  - maintenance endpoints

Verification:
- Production configuration enumerates all enabled interfaces.
- Review checks for accidental exposure.

---

## 14. Engineering checklists

### 14.1 Code review checklist
- No secrets in code or logs.
- All debug features behind config flags, default off.
- Cryptography uses ESP-IDF / mbedTLS primitives only.
- Any exception is documented and approved.

### 14.2 Release checklist
- Production config verified.
- Secure Boot enabled and images signed.
- Flash encryption enabled.
- NVS encryption enabled if secrets stored.
- Anti-rollback enabled.
- TLS configuration validated.

---

## 15. Phase alignment (when to enable what)

- Phase 0: security ready only (no enabling of secure boot or flash encryption). Ensure compatibility and no secret leakage.
- Phase 1 to Phase 3: maintain security rules and add secure storage abstractions as required.
- Phase 4: integration tests for TLS and certificate paths, if networking is implemented earlier.
- Phase 5: enable Secure Boot, Flash Encryption, NVS Encryption in production builds, and finalise manufacturing provisioning workflows.

---

## 16. Open items for review

- Confirm whether TLS is terminated on ESP32 or on modem, and how certificates and keys are stored for that path.
- Confirm provisioning station and signing service approach (CSR based recommended).
- Confirm whether a dedicated secure element is required in future revisions.

---
