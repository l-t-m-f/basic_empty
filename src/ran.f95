module random_module
    use, intrinsic :: iso_c_binding, only : c_int32_t, c_double
contains
    subroutine generate_random_number(random_value) bind(C, name = "generate_random_number")
        ! Declare the variables
        real(c_double) :: random_real
        integer(c_int32_t), intent(out) :: random_value

        ! Generate a random number
        call random_number(random_real)

        ! Convert the random number to the desired range (for example, 0 to 100)
        random_value = int(random_real * 100.0_c_double, c_int32_t)
    end subroutine generate_random_number
end module random_module
