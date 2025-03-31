use ph::{GetSize, phast};
use std::slice;

use std::os::raw::c_char;
use std::hint::black_box;

pub struct PhastWrapper {
    hash_func8: phast::Function<ph::seeds::Bits8>,
    hash_func: phast::Function<ph::seeds::BitsFast>,
    bits_per_seed: u8
}

#[no_mangle]
pub extern "C" fn createPhastStruct() -> *mut PhastWrapper {
    let struct_instance = PhastWrapper {
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
pub extern "C" fn constructPhast(struct_ptr: *mut PhastWrapper, keys_ptr: *mut Vec<&'static [u8]>,
                                 bits_per_seed: u8, bucket_size100: u16, threads_num: usize) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let keys = unsafe { &mut *keys_ptr };
    if bits_per_seed == 8 {
        struct_instance.hash_func8 = phast::Function::with_slice_bps_bs_threads_hash(
            &keys[..],
            ph::seeds::Bits8,
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        );
    } else {
        struct_instance.hash_func = phast::Function::with_slice_bps_bs_threads_hash(
            &keys[..],
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
    if struct_instance.bits_per_seed == 8 {
        struct_instance.hash_func8.get(key) as u64
    } else {
        struct_instance.hash_func.get(key) as u64
    }
}

#[no_mangle]
pub extern "C" fn queryPhastAll(struct_ptr: *mut PhastWrapper, keys_ptr: *mut Vec<&'static [u8]>) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let keys = unsafe { &mut *keys_ptr };
    if struct_instance.bits_per_seed == 8 {
        for key in keys {
            black_box(struct_instance.hash_func8.get(*key));
        }
    } else {
        for key in keys {
            black_box(struct_instance.hash_func.get(*key));
        }
    }
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
