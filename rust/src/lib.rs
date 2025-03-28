mod ph;
mod ptr_hash;

use std::os::raw::c_char;
use libc::strlen;
use std::{str, slice};

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




