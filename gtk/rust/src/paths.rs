/* Copyright (C) 2024 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#![allow(dead_code)]

use std::path::*;
use std::env;

pub const HANDBRAKE_SOCKET: &str = "handbrake.socket";

pub fn home_dir() -> PathBuf {
    #[allow(deprecated)]
    match env::home_dir() {
        Some(val) => val,
        None => PathBuf::from("/")
    }
}

pub fn xdg_config_home() -> PathBuf {
    match env::var("XDG_CONFIG_HOME") {
        Ok(var) => PathBuf::from(var),
        Err(_) => home_dir().join(".config")
    }
}

pub fn xdg_data_home() -> PathBuf {
    match env::var("XDG_DATA_HOME") {
        Ok(var) => PathBuf::from(var),
        Err(_) => home_dir().join(".local").join(".share")
    }
}

pub fn xdg_state_home() -> PathBuf {
    match env::var("XDG_STATE_HOME") {
        Ok(var) => PathBuf::from(var),
        Err(_) => home_dir().join(".local").join(".state")
    }
}

pub fn xdg_cache_home() -> PathBuf {
    match env::var("XDG_CACHE_HOME") {
        Ok(var) => PathBuf::from(var),
        Err(_) => home_dir().join(".cache")
    }
}

pub fn xdg_runtime_dir() -> PathBuf {
    match env::var("XDG_RUNTIME_DIR") {
        Ok(var) => PathBuf::from(var),
        Err(_) => xdg_cache_home()
    }
}

pub fn socket_path() -> PathBuf {
    xdg_runtime_dir().join(HANDBRAKE_SOCKET)
}
