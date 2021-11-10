module armie_fortran
   use iso_c_binding, only: C_INT
   interface 
     subroutine armie_start() BIND(C, name='armie_start') 
        use, intrinsic :: ISO_C_BINDING  
        implicit none 
     end subroutine armie_start

      subroutine armie_stop()  BIND(C, name='armie_stop')
        use, intrinsic :: ISO_C_BINDING  
        implicit none 
     end subroutine armie_stop
   end interface 

end module armie_fortran
