extern crate libc;
extern crate ph;
extern crate rayon;

use std::os::raw::c_char;
use libc::strlen;
use std::slice;
use std::str;
use ph::{fmph, GetSize, phast};
use ph::fmph::{BuildConf, GOBuildConf};
use mem_dbg::SizeFlags;
use ptr_hash::PtrHashParams;

////////////////////////////////////////// General methods //////////////////////////////////////////
fn c_strings_to_vec(len: usize, my_strings: *const *const c_char) -> Vec<String> {
    let mut vector = Vec::with_capacity(len);
    let sl = unsafe { std::slice::from_raw_parts(my_strings, len) };
    let mut index = 0;
    while index < sl.len() {
        let c_s = sl[index];
        let s = unsafe { str::from_utf8_unchecked(slice::from_raw_parts(c_s as *const u8, strlen(c_s)+1)) };
        vector.push(String::from(s));
        index += 1;
    }
    return vector;
}

fn c_strings_to_slices(len: usize, my_strings: *const *const c_char) -> Vec<&'static [u8]> {
    let mut vector = Vec::with_capacity(len);
    let sl = unsafe { std::slice::from_raw_parts(my_strings, len) };
    let mut index = 0;
    while index < sl.len() {
        let c_s = sl[index];
        let s = unsafe { slice::from_raw_parts(c_s as *const u8, strlen(c_s) + 1) };
        vector.push(s);
        index += 1;
    }
    return vector;
}

#[no_mangle]
pub extern "C" fn initializeRayonThreadPool(threads: usize) {
    rayon::ThreadPoolBuilder::new().num_threads(threads).build_global().unwrap();
}

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
            std::thread::available_parallelism().map_or(1, |v| v.into()),
            seedable_hash::BuildDefaultSeededHasher::default()
        ),
        bits_per_seed: 0
    };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructPhast(struct_ptr: *mut PhastWrapper, bits_per_seed: u8, bucket_size100: u16) {
    let struct_instance = unsafe { &mut *struct_ptr };
    if bits_per_seed == 8 {
        struct_instance.hash_func8 = phast::Function::with_slice_bps_bs_threads_hash(
            &struct_instance.vector[..],
            ph::seeds::Bits8,
            bucket_size100,
            std::thread::available_parallelism().map_or(1, |v| v.into()),
            seedable_hash::BuildDefaultSeededHasher::default()
        );
    } else {
        struct_instance.hash_func = phast::Function::with_slice_bps_bs_threads_hash(
            &struct_instance.vector[..],
            ph::seeds::BitsFast(bits_per_seed),
            bucket_size100,
            std::thread::available_parallelism().map_or(1, |v| v.into()),
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


//////////////////////////////////////////// PtrHash ///////////////////////////////////////////
type PtrHashLinearVec = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::Linear,
    Vec<u32>, ptr_hash::hash::Xx64, Vec<u8>>; // Fast
type PtrHashSquareVec = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::SquareEps,
    Vec<u32>, ptr_hash::hash::Xx64, Vec<u8>>;
type PtrHashCubicVec = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::CubicEps,
    Vec<u32>, ptr_hash::hash::Xx64, Vec<u8>>;
type PtrHashLinearEF = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::Linear,
    ptr_hash::CachelineEfVec, ptr_hash::hash::Xx64, Vec<u8>>;
type PtrHashSquareEF = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::SquareEps,
    ptr_hash::CachelineEfVec, ptr_hash::hash::Xx64, Vec<u8>>;
type PtrHashCubicEF = ptr_hash::PtrHash<&'static [u8], ptr_hash::bucket_fn::CubicEps,
    ptr_hash::CachelineEfVec, ptr_hash::hash::Xx64, Vec<u8>>; // Compact

pub struct PtrHashWrapper {
    vector: Vec<&'static [u8]>,
    variant: u64,
    ptrhash_linear_vec: PtrHashLinearVec,
    ptrhash_square_vec: PtrHashSquareVec,
    ptrhash_cubic_vec: PtrHashCubicVec,
    ptrhash_linear_ef: PtrHashLinearEF,
    ptrhash_square_ef: PtrHashSquareEF,
    ptrhash_cubic_ef: PtrHashCubicEF,
}

#[no_mangle]
pub extern "C" fn createPtrHashStruct(len: usize, my_strings: *const *const c_char) -> *mut PtrHashWrapper {
    let struct_instance = PtrHashWrapper {
        vector: c_strings_to_slices(len, my_strings),
        variant: 0,
        ptrhash_linear_vec: ptr_hash::PtrHash::new(&[], PtrHashParams::default_fast()),
        ptrhash_square_vec: ptr_hash::PtrHash::new(&[], PtrHashParams::default_square()),
        ptrhash_cubic_vec: ptr_hash::PtrHash::new(&[], PtrHashParams::default_compact()),
        ptrhash_linear_ef: ptr_hash::PtrHash::new(&[], PtrHashParams::default_fast()),
        ptrhash_square_ef: ptr_hash::PtrHash::new(&[], PtrHashParams::default_square()),
        ptrhash_cubic_ef: ptr_hash::PtrHash::new(&[], PtrHashParams::default_compact()),
    };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructPtrHash(struct_ptr: *mut PtrHashWrapper, variant : u64, lambda : f64) {
    let struct_instance = unsafe { &mut *struct_ptr };
    struct_instance.variant = variant;
    match variant {
        1 => {
            let mut params = PtrHashParams::default_fast();
            params.lambda = lambda;
            struct_instance.ptrhash_linear_vec = PtrHashLinearVec::new(&struct_instance.vector[..], params);
        },
        2 => {
            let mut params = PtrHashParams::default_square();
            params.lambda = lambda;
            struct_instance.ptrhash_square_vec = PtrHashSquareVec::new(&struct_instance.vector[..], params);
        },
        3 => {
            let mut params = PtrHashParams::default_compact();
            params.lambda = lambda;
            struct_instance.ptrhash_cubic_vec = PtrHashCubicVec::new(&struct_instance.vector[..], params);
        },
        4 => {
            let mut params = PtrHashParams::default_fast();
            params.lambda = lambda;
            struct_instance.ptrhash_linear_ef = PtrHashLinearEF::new(&struct_instance.vector[..], params);
        },
        5 => {
            let mut params = PtrHashParams::default_square();
            params.lambda = lambda;
            struct_instance.ptrhash_square_ef = PtrHashSquareEF::new(&struct_instance.vector[..], params);
        },
        6 => {
            let mut params = PtrHashParams::default_compact();
            params.lambda = lambda;
            struct_instance.ptrhash_cubic_ef = PtrHashCubicEF::new(&struct_instance.vector[..], params);
        },
        _ => panic!("Invalid variant"),
    }
}

#[no_mangle]
pub extern "C" fn queryPtrHash1(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_linear_vec.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn queryPtrHash2(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_square_vec.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn queryPtrHash3(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_cubic_vec.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn queryPtrHash4(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_linear_ef.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn queryPtrHash5(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_square_ef.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn queryPtrHash6(struct_ptr: *mut PtrHashWrapper, key_c_s : *const c_char, length : usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    struct_instance.ptrhash_cubic_ef.index_minimal(&key) as u64
}

#[no_mangle]
pub extern "C" fn sizePtrHash(struct_ptr: *mut PtrHashWrapper) -> usize {
    let struct_instance = unsafe { &mut *struct_ptr };
    use mem_dbg::MemSize;
    match struct_instance.variant {
        1 => struct_instance.ptrhash_linear_vec.mem_size(SizeFlags::default()),
        2 => struct_instance.ptrhash_square_vec.mem_size(SizeFlags::default()),
        3 => struct_instance.ptrhash_cubic_vec.mem_size(SizeFlags::default()),
        4 => struct_instance.ptrhash_linear_ef.mem_size(SizeFlags::default()),
        5 => struct_instance.ptrhash_square_ef.mem_size(SizeFlags::default()),
        6 => struct_instance.ptrhash_cubic_ef.mem_size(SizeFlags::default()),
        _ => panic!("Invalid variant"),
    }
}

#[no_mangle]
pub extern "C" fn destroyPtrHashStruct(struct_instance: *mut PtrHashWrapper) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
