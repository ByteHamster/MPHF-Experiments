extern crate libc;

use std::os::raw::c_char;
use libc::strlen;
use std::slice;
use std::str;

pub struct S {
    my_int: u64
}

#[no_mangle]
pub extern fn createStruct(len: usize, my_strings: *const *const c_char) -> *mut S {
    let struct_instance = S { my_int: 42 };
    unsafe {
        let sl = std::slice::from_raw_parts(my_strings, len);
        let mut index = 0;
        while index < sl.len() {
            let c_s = sl[index];
            let s = str::from_utf8_unchecked(slice::from_raw_parts(c_s as *const u8, strlen(c_s)+1));
            println!("Input string: {}", s);
            index += 1;
        }
    };

    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern fn useStruct(struct_ptr: *mut S) {
    let struct_instance = unsafe { &mut *struct_ptr };
    println!("Use struct: {}", struct_instance.my_int);
}

#[no_mangle]
pub extern fn destroyStruct(struct_instance: *mut S) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
