use core::alloc::Layout;
use core::ffi::c_void;
use std::alloc;

fn align_up(size: usize, align: usize) -> usize {
    (size + align - 1) & !(align - 1)
}

#[unsafe(no_mangle)]
unsafe extern "C" fn malloc(size: usize) -> *mut c_void {
    unsafe {
        let size = align_up(size + size_of::<usize>(), 16);
        let layout = Layout::from_size_align(size, 16).unwrap();
        let ptr = alloc::alloc(layout);
        if ptr.is_null() {
            return ptr.cast();
        }
        ptr.cast::<usize>().write(size);
        ptr.byte_add(size_of::<usize>()).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn calloc(num: usize, size: usize) -> *mut c_void {
    let size = num * size;
    let size = align_up(size + size_of::<usize>(), 16);
    unsafe {
        let layout = Layout::from_size_align(size, 16).unwrap();
        let ptr = alloc::alloc_zeroed(layout);
        if ptr.is_null() {
            return ptr.cast();
        }
        ptr.cast::<usize>().write(size);
        ptr.byte_add(size_of::<usize>()).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn realloc(ptr: *mut c_void, new: usize) -> *mut c_void {
    unsafe {
        if ptr.is_null() {
            return malloc(new);
        }
        let new = align_up(new + size_of::<usize>(), 16);
        let old = ptr.byte_sub(size_of::<usize>());
        let size = old.cast::<usize>().read();
        let layout = Layout::from_size_align(size, 16).unwrap();
        let ptr = alloc::realloc(old.cast(), layout, new);
        if ptr.is_null() {
            return ptr.cast();
        }
        ptr.cast::<usize>().write(new);
        ptr.byte_add(size_of::<usize>()).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn free(ptr: *mut c_void) {
    if ptr.is_null() {
        return;
    }
    unsafe {
        let ptr = ptr.byte_sub(size_of::<usize>());
        let size = ptr.cast::<usize>().read();
        let layout = Layout::from_size_align(size, 16).unwrap();
        alloc::dealloc(ptr.cast(), layout);
    }
}
