use ph::{fmph, GetSize};
use ph::fmph::BuildConf;
use std::{str, slice};

use crate::c_strings_to_vec;
use std::os::raw::c_char;

pub struct FmphWrapper {
    vector: Vec<String>,
    hash_func: fmph::Function
}

#[no_mangle]
pub extern "C" fn createFmphStruct(len: usize, my_strings: *const *const c_char) -> *mut FmphWrapper {
    let struct_instance = FmphWrapper {
        vector: c_strings_to_vec(len, my_strings),
        hash_func: fmph::Function::from(&[] as &[String]) };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructFmph(struct_ptr: *mut FmphWrapper, gamma : u16) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let mut build_config = BuildConf::default();
    build_config.use_multiple_threads = true;
    build_config.relative_level_size = gamma;
    struct_instance.hash_func = fmph::Function::from_slice_with_conf(&struct_instance.vector[..], build_config);
}

#[no_mangle]
pub extern "C" fn queryFmph(struct_ptr: *mut FmphWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { str::from_utf8_unchecked(slice::from_raw_parts(key_c_s as *const u8, length+1)) };
    struct_instance.hash_func.get(key).unwrap()
}

#[no_mangle]
pub extern "C" fn sizeFmph(struct_ptr: *mut FmphWrapper) -> usize {
    let struct_instance = unsafe { &mut *struct_ptr };
    struct_instance.hash_func.size_bytes()
}

#[no_mangle]
pub extern "C" fn destroyFmphStruct(struct_instance: *mut FmphWrapper) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
