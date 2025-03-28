use ph::{fmph, GetSize, phast};
use ph::fmph::{BuildConf, GOBuildConf};
use std::{str, slice};

use crate::{c_strings_to_slices, c_strings_to_vec};
use std::os::raw::c_char;

////////////////////////////////////////// Fmph //////////////////////////////////////////
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

////////////////////////////////////////// FmphGO //////////////////////////////////////////
pub struct FmphGoWrapper {
    vector: Vec<String>,
    hash_func: fmph::GOFunction
}

#[no_mangle]
pub extern "C" fn createFmphGoStruct(len: usize, my_strings: *const *const c_char) -> *mut FmphGoWrapper {
    let struct_instance = FmphGoWrapper {
        vector: c_strings_to_vec(len, my_strings),
        hash_func: fmph::GOFunction::from(&[] as &[String]) };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructFmphGo(struct_ptr: *mut FmphGoWrapper, gamma : u16) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let mut build_config = GOBuildConf::default();
    build_config.use_multiple_threads = true;
    build_config.relative_level_size = gamma;
    struct_instance.hash_func = fmph::GOFunction::from_slice_with_conf(&struct_instance.vector[..], build_config);
}

#[no_mangle]
pub extern "C" fn queryFmphGo(struct_ptr: *mut FmphGoWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { str::from_utf8_unchecked(slice::from_raw_parts(key_c_s as *const u8, length+1)) };
    struct_instance.hash_func.get(key).unwrap()
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

////////////////////////////////////////// PHast //////////////////////////////////////////
pub struct PhastWrapper {
    vector: Vec<&'static [u8]>,
    hash_func8: phast::Function<ph::seeds::Bits8>,
    hash_func: phast::Function<ph::seeds::BitsFast>,
    bits_per_seed: u8
}

#[no_mangle]
pub extern "C" fn createPhastStruct(len: usize, my_strings: *const *const c_char) -> *mut PhastWrapper {
    let struct_instance = PhastWrapper {
        vector: c_strings_to_slices(len, my_strings),
        hash_func8: phast::Function::from_slice_mt(&[] as &[&[u8]]),
        hash_func: phast::Function::with_slice_bps_bs_threads_hash(
            &[] as &[&[u8]],
            ph::seeds::BitsFast(4),
            300,
            1,
            seedable_hash::BuildDefaultSeededHasher::default()
        ),
        bits_per_seed: 0
    };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructPhast(struct_ptr: *mut PhastWrapper, bits_per_seed: u8, bucket_size100: u16, threads_num: usize) {
    let struct_instance = unsafe { &mut *struct_ptr };
    if bits_per_seed == 8 {
        struct_instance.hash_func8 = phast::Function::with_slice_bps_bs_threads_hash(
            &struct_instance.vector[..],
            ph::seeds::Bits8,
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        );
    } else {
        struct_instance.hash_func = phast::Function::with_slice_bps_bs_threads_hash(
            &struct_instance.vector[..],
            ph::seeds::BitsFast(bits_per_seed),
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        );
    }
    struct_instance.bits_per_seed = bits_per_seed;
}

#[no_mangle]
pub extern "C" fn queryPhast(struct_ptr: *mut PhastWrapper, key_c_s: *const c_char, length: usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.hash_func.get(key) as u64
}

#[no_mangle]
pub extern "C" fn queryPhast8(struct_ptr: *mut PhastWrapper, key_c_s: *const c_char, length: usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.hash_func8.get(key) as u64
}

#[no_mangle]
pub extern "C" fn sizePhast(struct_ptr: *mut PhastWrapper) -> usize {
    let struct_instance = unsafe { &mut *struct_ptr };
    if struct_instance.bits_per_seed == 8 {
        struct_instance.hash_func8.size_bytes()
    } else {
        struct_instance.hash_func.size_bytes()
    }
}

#[no_mangle]
pub extern "C" fn destroyPhastStruct(struct_instance: *mut PhastWrapper) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}