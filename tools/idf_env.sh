#!/usr/bin/env bash

# This script is intended to be sourced from an interactive shell.
# It works from both bash and zsh and does not mutate shell options.

_idf_env_die() {
    echo "ERROR: $*" >&2
    return 1 2>/dev/null || exit 1
}

_idf_env_repo_root() {
    local root=""

    # Preferred path: resolve repo root from git regardless of caller shell.
    if command -v git >/dev/null 2>&1; then
        root="$(git rev-parse --show-toplevel 2>/dev/null || true)"
    fi

    # Fallback for bash.
    if [ -z "${root}" ] && [ -n "${BASH_SOURCE[0]-}" ]; then
        root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." >/dev/null 2>&1 && pwd || true)"
    fi

    # Fallback for zsh (`%x` is the sourced file path).
    if [ -z "${root}" ] && [ -n "${ZSH_VERSION-}" ]; then
        local zsh_path=""
        zsh_path="$(eval 'printf "%s" "${(%):-%x}"' 2>/dev/null || true)"
        if [ -n "${zsh_path}" ]; then
            root="$(cd "$(dirname "${zsh_path}")/.." >/dev/null 2>&1 && pwd || true)"
        fi
    fi

    [ -n "${root}" ] || return 1
    printf '%s\n' "${root}"
}

REPO_ROOT="$(_idf_env_repo_root)" || _idf_env_die "Unable to determine repository root. Run this from inside the repo."
IDF="${REPO_ROOT}/third_party/esp-idf"

[ -d "${IDF}" ] || _idf_env_die "ESP-IDF directory not found at ${IDF}"
[ -f "${IDF}/export.sh" ] || _idf_env_die "ESP-IDF export script not found at ${IDF}/export.sh"

# shellcheck disable=SC1090
. "${IDF}/export.sh"
