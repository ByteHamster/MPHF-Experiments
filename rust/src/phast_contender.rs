use ph::{GetSize, phast};
use std::slice;

use std::os::raw::c_char;
use std::hint::black_box;

pub enum PHastVariant {
    Bits(phast::Function<ph::seeds::BitsFast>),
    Bits8(phast::Function<ph::seeds::Bits8>),
    //Bits4(phast::Function<ph::seeds::TwoToPowerBitsStatic::<2>>),
    None
}

#[no_mangle]
pub extern "C" fn createPhastStruct() -> *mut PHastVariant {
    Box::into_raw(Box::new(PHastVariant::None))
}

#[no_mangle]
pub extern "C" fn constructPhast(struct_ptr: *mut PHastVariant, keys_ptr: *const Box<[&'static [u8]]>,
                                 bits_per_seed: u8, bucket_size100: u16, threads_num: usize) {
    let f = unsafe { &mut *struct_ptr };
    let keys = unsafe { &*keys_ptr };
    *f = match bits_per_seed {
        8 => PHastVariant::Bits8(phast::Function::with_slice_bps_bs_threads_hash(
            &keys[..],
            ph::seeds::Bits8,
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        )),
        /*4 => PHastVariant::Bits4(phast::Function::with_slice_bps_bs_threads_hash(
            &keys[..],
            ph::seeds::TwoToPowerBitsStatic::<2>,
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        )),*/
        _ => PHastVariant::Bits(phast::Function::with_slice_bps_bs_threads_hash(
            &keys[..],
            ph::seeds::BitsFast(bits_per_seed),
            bucket_size100,
            threads_num,
            seedable_hash::BuildDefaultSeededHasher::default()
        )),
    }
}

#[no_mangle]
pub extern "C" fn queryPhast(struct_ptr: *const PHastVariant, key_c_s: *const c_char, length: usize) -> u64 {
    let f = unsafe { &*struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length + 1) };
    match f {
        PHastVariant::Bits(function) => function.get(key) as u64,
        PHastVariant::Bits8(function) => function.get(key) as u64,
        //PHastVariant::Bits4(function) => function.get(key) as u64,
        PHastVariant::None => panic!("PHast not constructed yet"),
    }
}

#[no_mangle]
pub extern "C" fn queryPhastAll(struct_ptr: *const PHastVariant, keys_ptr: *const Box<[&'static [u8]]>) {
    let f = unsafe { &*struct_ptr };
    let keys = unsafe { &*keys_ptr };
    match f {
        PHastVariant::Bits(function) =>
            for key in keys { black_box(function.get(*key)); },
        PHastVariant::Bits8(function) => 
            for key in keys { black_box(function.get(*key)); },
        /*PHastVariant::Bits4(function) => 
            for key in keys { black_box(function.get(*key)); },*/
        PHastVariant::None => panic!("PHast not constructed yet"),
    }
}

#[no_mangle]
pub extern "C" fn sizePhast(struct_ptr: *const PHastVariant) -> usize {
    match unsafe { &*struct_ptr } {
        PHastVariant::Bits(function) => function.size_bytes(),
        PHastVariant::Bits8(function) => function.size_bytes(),
        //PHastVariant::Bits4(function) => function.size_bytes(),
        PHastVariant::None => panic!("PHast not constructed yet"),
    }
}

#[no_mangle]
pub extern "C" fn destroyPhastStruct(struct_instance: *mut PHastVariant) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
