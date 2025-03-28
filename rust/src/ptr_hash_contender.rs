use mem_dbg::SizeFlags;
use ptr_hash::PtrHashParams;

use std::os::raw::c_char;
use std::slice;
use std::hint::black_box;

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

enum PtrHashVariant {
    None,
    LinearVec(PtrHashLinearVec),
    SquareVec(PtrHashSquareVec),
    CubicVec(PtrHashCubicVec),
    LinearEF(PtrHashLinearEF),
    SquareEF(PtrHashSquareEF),
    CubicEF(PtrHashCubicEF),
}

pub struct PtrHashWrapper {
    func: PtrHashVariant
}

#[no_mangle]
pub extern "C" fn createPtrHashStruct() -> *mut PtrHashWrapper {
    let struct_instance = PtrHashWrapper {
        func: PtrHashVariant::None
    };
    let boxx = Box::new(struct_instance);
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn constructPtrHash(struct_ptr: *mut PtrHashWrapper, keys_ptr: *mut Vec<&'static [u8]>, variant: u64, lambda: f64) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let keys = unsafe { &mut *keys_ptr };
    struct_instance.func = match variant {
        1 => {
            let mut params = PtrHashParams::default_fast();
            params.lambda = lambda;
            PtrHashVariant::LinearVec( PtrHashLinearVec::new(&keys[..], params) )
        },
        2 => {
            let mut params = PtrHashParams::default_square();
            params.lambda = lambda;
            PtrHashVariant::SquareVec( PtrHashSquareVec::new(&keys[..], params) )
        },
        3 => {
            let mut params = PtrHashParams::default_compact();
            params.lambda = lambda;
            PtrHashVariant::CubicVec( PtrHashCubicVec::new(&keys[..], params) )
        },
        4 => {
            let mut params = PtrHashParams::default_fast();
            params.lambda = lambda;
            PtrHashVariant::LinearEF( PtrHashLinearEF::new(&keys[..], params) )
        },
        5 => {
            let mut params = PtrHashParams::default_square();
            params.lambda = lambda;
            PtrHashVariant::SquareEF( PtrHashSquareEF::new(&keys[..], params) )
        },
        6 => {
            let mut params = PtrHashParams::default_compact();
            params.lambda = lambda;
            PtrHashVariant::CubicEF( PtrHashCubicEF::new(&keys[..], params) )
        },
        _ => panic!("Invalid variant"),
    }
}

#[no_mangle]
pub extern "C" fn queryPtrHash(struct_ptr: *mut PtrHashWrapper, key_c_s: *const c_char, length: usize) -> u64 {
    let struct_instance = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    match &struct_instance.func {
        PtrHashVariant::LinearVec(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::SquareVec(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::CubicVec(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::LinearEF(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::SquareEF(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::CubicEF(ref f) => f.index_minimal(&key) as u64,
        PtrHashVariant::None => panic!("PtrHash not constructed yet"),
    }
}

#[no_mangle]
pub extern "C" fn queryPtrHashAll(struct_ptr: *mut PtrHashWrapper, keys_ptr: *mut Vec<&'static [u8]>) {
    let struct_instance = unsafe { &mut *struct_ptr };
    let keys = unsafe { &mut *keys_ptr };
    for key in keys {
        let result = match &struct_instance.func {
            PtrHashVariant::LinearVec(ref f) => f.index_minimal(&key),
            PtrHashVariant::SquareVec(ref f) => f.index_minimal(&key),
            PtrHashVariant::CubicVec(ref f) => f.index_minimal(&key),
            PtrHashVariant::LinearEF(ref f) => f.index_minimal(&key),
            PtrHashVariant::SquareEF(ref f) => f.index_minimal(&key),
            PtrHashVariant::CubicEF(ref f) => f.index_minimal(&key),
            PtrHashVariant::None => panic!("PtrHash not constructed yet"),
        };
        black_box(result);
    }
}

#[no_mangle]
pub extern "C" fn sizePtrHash(struct_ptr: *mut PtrHashWrapper) -> usize {
    let struct_instance = unsafe { &mut *struct_ptr };
    use mem_dbg::MemSize;
    match &struct_instance.func {   
        PtrHashVariant::LinearVec(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::SquareVec(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::CubicVec(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::LinearEF(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::SquareEF(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::CubicEF(f) => f.mem_size(SizeFlags::default()),
        PtrHashVariant::None => panic!("PtrHash not constructed yet"),
    }
}

#[no_mangle]
pub extern "C" fn destroyPtrHashStruct(struct_instance: *mut PtrHashWrapper) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
