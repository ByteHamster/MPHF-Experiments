use ph::{fmph, GetSize};
use ph::fmph::GOBuildConf;
use std::{str, slice};

use std::os::raw::c_char;
use std::hint::black_box;

pub struct FmphGoWrapper {
    hash_func: fmph::GOFunction
}

#[no_mangle]
pub extern "C" fn createFmphGoStruct() -> *mut FmphGoWrapper {
    let struct_instance = FmphGoWrapper {
        hash_func: fmph::GOFunction::from(&[] as &[String]) };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructFmphGo(struct_ptr: *mut FmphGoWrapper, keys_ptr: *mut Vec<String>, gamma: u16) {
    let keys = unsafe { &mut *keys_ptr };
    let struct_instance = unsafe { &mut *struct_ptr };
    let mut build_config = GOBuildConf::default();
    build_config.use_multiple_threads = true;
    build_config.relative_level_size = gamma;
    struct_instance.hash_func = fmph::GOFunction::from_slice_with_conf(&keys[..], build_config);
}

#[no_mangle]
pub extern "C" fn queryFmphGo(struct_ptr: *mut FmphGoWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { str::from_utf8_unchecked(slice::from_raw_parts(key_c_s as *const u8, length+1)) };
    struct_instance.hash_func.get(key).unwrap()
}

#[no_mangle]
pub extern "C" fn queryFmphGoAll(struct_ptr: *mut FmphGoWrapper, keys_ptr: *mut Vec<String>) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let keys = unsafe { &mut *keys_ptr };
    for key in keys {
        black_box(struct_instance.hash_func.get(key).unwrap());
    }
}

#[no_mangle]
pub extern "C" fn sizeFmphGo(struct_ptr: *mut FmphGoWrapper) -> usize {
    let struct_instance = unsafe { &mut *struct_ptr };
    struct_instance.hash_func.size_bytes()
}

#[no_mangle]
pub extern "C" fn destroyFmphGoStruct(struct_instance: *mut FmphGoWrapper) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
