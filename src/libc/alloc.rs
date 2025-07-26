use core::ffi::c_void;
use core::alloc::Layout;
use std::alloc;

fn align_up(size: usize, align: usize) -> usize {
    (size + align - 1) & !(align - 1)
}

#[unsafe(no_mangle)]
unsafe extern "C" fn malloc(size: usize) -> *mut c_void {
    unsafe {
        let layout = Layout::from_size_align_unchecked(align_up(size + size_of::<usize>(), 16), 16);
        let ptr = alloc::alloc(layout);
        ptr.cast::<usize>().write(size + size_of::<usize>());
        ptr.byte_offset(size_of::<usize>() as isize).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn calloc(num: usize, size: usize) -> *mut c_void {
    let size = num * size;
    unsafe {
        let layout = Layout::from_size_align_unchecked(align_up(size + size_of::<usize>(), 16), 16);
        let ptr = alloc::alloc_zeroed(layout);
        ptr.cast::<usize>().write(size + size_of::<usize>());
        ptr.byte_offset(size_of::<usize>() as isize).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn realloc(ptr: *mut c_void, new: usize) -> *mut c_void {
    unsafe {
        if ptr.is_null() {
            return malloc(new);
        }
        let ptr = ptr.offset(-(size_of::<usize>() as isize));
        let size = ptr.cast::<usize>().read();
        let layout = Layout::from_size_align_unchecked(align_up(size, 16), 16);
        let ptr = alloc::realloc(ptr.cast(), layout, new + size_of::<usize>());
        ptr.cast::<usize>().write(new + size_of::<usize>());
        ptr.byte_offset(size_of::<usize>() as isize).cast()
    }
}
#[unsafe(no_mangle)]
unsafe extern "C" fn free(ptr: *mut c_void) {
    unsafe {
        let ptr = ptr.offset(-(size_of::<usize>() as isize));
        let size = ptr.cast::<usize>().read();
        let layout = Layout::from_size_align_unchecked(size, 16);
        alloc::dealloc(ptr.cast(), layout);
    }
}
