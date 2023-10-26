/* Copyright (C) 2024 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

use crate::paths;

#[no_mangle]
pub extern "C" fn rust_hello1() {
    println!("Socket path: {}", paths::socket_path().display());
}

pub fn hello () {
    println!("Hello there.");
}


