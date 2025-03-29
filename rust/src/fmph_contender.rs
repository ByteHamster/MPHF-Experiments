use ph::{fmph, GetSize};
use ph::fmph::BuildConf;
use std::{str, slice};

use std::os::raw::c_char;
use std::hint::black_box;

#[no_mangle]
pub extern "C" fn createFmphStruct() -> *mut fmph::Function {
    Box::into_raw(Box::new(fmph::Function::from(&[] as &[&'static [u8]])))
}

#[no_mangle]
pub extern "C" fn constructFmph(struct_ptr: *mut fmph::Function, keys_ptr: *const Box<[&'static [u8]]>, gamma: u16) {
    let keys = unsafe { &*keys_ptr };
    let f = unsafe { &mut *struct_ptr };
    let mut build_config = BuildConf::default();
    build_config.use_multiple_threads = true;
    build_config.relative_level_size = gamma;
    *f = fmph::Function::from_slice_with_conf(&keys[..], build_config);
}

#[no_mangle]
pub extern "C" fn queryFmph(struct_ptr: *mut fmph::Function, key_c_s: *const c_char, length: usize) -> u64 {
    let f = unsafe { &mut *struct_ptr };
    let key = unsafe { slice::from_raw_parts(key_c_s as *const u8, length+1) };
    f.get_or_panic(key)
}

#[no_mangle]
pub extern "C" fn queryFmphAll(struct_ptr: *mut fmph::Function, keys_ptr: *const Box<[&'static [u8]]>) {
    let f = unsafe { &mut *struct_ptr };
    let keys = unsafe { &*keys_ptr };
    for key in keys {
        black_box(f.get(key).unwrap());
    }
}

#[no_mangle]
pub extern "C" fn sizeFmph(struct_ptr: *mut fmph::Function) -> usize {
    unsafe { &mut *struct_ptr }.size_bytes()
}

#[no_mangle]
pub extern "C" fn destroyFmphStruct(struct_instance: *mut fmph::Function) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
