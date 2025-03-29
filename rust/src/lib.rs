mod fmph_contender;
mod fmph_go_contender;
mod phast_contender;
mod ptr_hash_contender;

use std::os::raw::c_char;
use libc::strlen;
use std::slice;

#[no_mangle]
pub extern "C" fn initializeRayonThreadPool(threads: usize) {
    rayon::ThreadPoolBuilder::new().num_threads(threads).build_global().unwrap();
}

#[no_mangle]
pub extern "C" fn convertToVecSlice(len: usize, my_strings: *const *const c_char) -> *mut Box<[&'static [u8]]> {
    let mut keys = Vec::with_capacity(len);
    let sl = unsafe { std::slice::from_raw_parts(my_strings, len) };
    let mut index = 0;
    while index < sl.len() {
        let c_s = sl[index];
        let s = unsafe { slice::from_raw_parts(c_s as *const u8, strlen(c_s) + 1) };
        keys.push(s);
        index += 1;
    }
    let boxx = Box::new(keys.into_boxed_slice());
    Box::into_raw(boxx)
}

#[no_mangle]
pub extern "C" fn destroyVecSlice(struct_instance: *mut Box<[&'static [u8]]>) {
    unsafe { let _ = Box::from_raw(struct_instance); }
}
