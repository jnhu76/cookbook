pub fn phf_shift(x: u32) -> u8 {
    let shift = x.wrapping_mul(0xa463293e) >> 27;
    ((0x824a1847u32 >> shift) & 0b11111) as u8
}



#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = phf_shift(32145215u32);
        assert_eq!(result, 18);
    }
}
