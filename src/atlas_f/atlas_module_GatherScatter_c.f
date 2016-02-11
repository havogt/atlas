! (C) Copyright 2013-2015 ECMWF.

! ------------------------------------------------------------------------------
! Gather routines

function atlas_GatherScatter__ctor() result(gather)
  type(atlas_GatherScatter) :: gather
  call gather%reset_c_ptr( atlas__GatherScatter__new() )
end function atlas_GatherScatter__ctor

subroutine atlas_GatherScatter__delete(this)
  class(atlas_GatherScatter), intent(inout) :: this
  if ( .not. this%is_null() ) then
    call atlas__GatherScatter__delete(this%c_ptr())
  end if
  call this%reset_c_ptr()
end subroutine atlas_GatherScatter__delete


subroutine atlas_GatherScatter__copy(this,obj_in)
  class(atlas_GatherScatter), intent(inout) :: this
  class(atlas_RefCounted), target, intent(in) :: obj_in
end subroutine

subroutine GatherScatter__setup32(this, part, remote_idx, glb_idx, opt_max_glb_idx)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in) :: part(:)
  integer(c_int), intent(in) :: remote_idx(:)
  integer(c_int), intent(in) :: glb_idx(:)
  integer(c_int), optional, intent(in) :: opt_max_glb_idx
  integer(c_int) :: max_glb_idx
  if (.not. present(opt_max_glb_idx) ) then
    max_glb_idx = huge(max_glb_idx)
  else
    max_glb_idx = opt_max_glb_idx
  endif
  call atlas__GatherScatter__setup32( this%c_ptr(), part, remote_idx, 1, &
    &                        glb_idx, max_glb_idx, size(part) )
end subroutine GatherScatter__setup32

subroutine GatherScatter__setup64(this, part, remote_idx, glb_idx, opt_max_glb_idx)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in) :: part(:)
  integer(c_int), intent(in) :: remote_idx(:)
  integer(c_long), intent(in) :: glb_idx(:)
  integer(c_long), optional, intent(in) :: opt_max_glb_idx
  integer(c_long) :: max_glb_idx
  if (.not. present(opt_max_glb_idx) ) then
    max_glb_idx = huge(max_glb_idx)
  else
    max_glb_idx = opt_max_glb_idx
  endif
  call atlas__GatherScatter__setup64( this%c_ptr(), part, remote_idx, 1, &
    &                        glb_idx, max_glb_idx, size(part) )
end subroutine GatherScatter__setup64

function GatherScatter__glb_dof(this) result(glb_dof)
  class(atlas_GatherScatter), intent(in) :: this
  integer :: glb_dof
  glb_dof = atlas__GatherScatter__glb_dof(this%c_ptr())
end function GatherScatter__glb_dof

subroutine GatherScatter__gather_int32_r1_r1(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in)  :: loc_field_data(:)
  integer(c_int), intent(out) :: glb_field_data(:)
  integer(c_int), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,2) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_int( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int32_r1_r1


subroutine GatherScatter__gather_int32_r2_r2(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in)  :: loc_field_data(:,:)
  integer(c_int), intent(out) :: glb_field_data(:,:)
  integer(c_int), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_int( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int32_r2_r2


subroutine GatherScatter__gather_int32_r3_r3(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in)  :: loc_field_data(:,:,:)
  integer(c_int), intent(out) :: glb_field_data(:,:,:)
  integer(c_int), pointer :: lview(:), gview(:)
  integer :: lstrides(3), lextents(3), lrank=3
  integer :: gstrides(3), gextents(3), grank=3
  lstrides = (/ stride(loc_field_data,3), stride(loc_field_data,2) , stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,2) , size(loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,3), stride(glb_field_data,2) , stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,2) , size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_int( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int32_r3_r3

subroutine GatherScatter__gather_int64_r1_r1(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_long), intent(in)  :: loc_field_data(:)
  integer(c_long), intent(out) :: glb_field_data(:)
  integer(c_long), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,2) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_long( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int64_r1_r1


subroutine GatherScatter__gather_int64_r2_r2(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_long), intent(in)  :: loc_field_data(:,:)
  integer(c_long), intent(out) :: glb_field_data(:,:)
  integer(c_long), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_long( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int64_r2_r2


subroutine GatherScatter__gather_int64_r3_r3(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_long), intent(in)  :: loc_field_data(:,:,:)
  integer(c_long), intent(out) :: glb_field_data(:,:,:)
  integer(c_long), pointer :: lview(:), gview(:)
  integer :: lstrides(3), lextents(3), lrank=3
  integer :: gstrides(3), gextents(3), grank=3
  lstrides = (/ stride(loc_field_data,3), stride(loc_field_data,2) , stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,2) , size(loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,3), stride(glb_field_data,2) , stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,2) , size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_long( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_int64_r3_r3


subroutine GatherScatter__gather_real32_r1_r1(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_float), intent(in)  :: loc_field_data(:)
  real(c_float), intent(out) :: glb_field_data(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  real(c_float), pointer :: lview(:), gview(:)
  lstrides = (/ stride(loc_field_data,2) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_float( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real32_r1_r1
subroutine GatherScatter__gather_real32_r2_r2(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_float), intent(in)  :: loc_field_data(:,:)
  real(c_float), intent(out) :: glb_field_data(:,:)
  real(c_float), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_float( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real32_r2_r2
subroutine GatherScatter__gather_real32_r3_r3(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_float), intent(in)  :: loc_field_data(:,:,:)
  real(c_float), intent(out) :: glb_field_data(:,:,:)
  real(c_float), pointer :: lview(:), gview(:)
  integer :: lstrides(3), lextents(3), lrank=3
  integer :: gstrides(3), gextents(3), grank=3
  lstrides = (/ stride(loc_field_data,3), stride(loc_field_data,2) , stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,2) , size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,3), stride(glb_field_data,2) , stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,2) , size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_float( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real32_r3_r3

subroutine GatherScatter__gather_real64_r1_r1(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)   :: loc_field_data(:)
  real(c_double), intent(out)  :: glb_field_data(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  real(c_double), pointer :: lview(:), gview(:)
  lstrides = (/ stride(loc_field_data,1) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,1) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  !write(0,*) atlas_mpi_rank(),"lstrides",lstrides
  !write(0,*) atlas_mpi_rank(),"lextents",lextents
  !write(0,*) atlas_mpi_rank(),"gstrides",gstrides
  !write(0,*) atlas_mpi_rank(),"gextents",gextents
  !write(0,*) atlas_mpi_rank(),"localsize",lstrides(1)*lextents(1)*size(loc_field_data)
  !write(0,*) "loc address, size = ",loc(loc_field_data(1)),size(loc_field_data), loc(lview(1))
  !write(0,*) "glb address, size = ",loc(gview(1)),size(gview)

  call atlas__GatherScatter__gather_double( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real64_r1_r1
subroutine GatherScatter__gather_real64_r2_r2(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)  :: loc_field_data(:,:)
  real(c_double), intent(out) :: glb_field_data(:,:)
  real(c_double), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_double( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real64_r2_r2
subroutine GatherScatter__gather_real64_r3_r3(this, loc_field_data, glb_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)  :: loc_field_data(:,:,:)
  real(c_double), intent(out) :: glb_field_data(:,:,:)
  real(c_double), pointer :: lview(:), gview(:)
  integer :: lstrides(3), lextents(3), lrank=3
  integer :: gstrides(3), gextents(3), grank=3
  lstrides = (/ stride(loc_field_data,3), stride(loc_field_data,2) , stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,2) , size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,3), stride(glb_field_data,2) , stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,2) , size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__gather_double( this%c_ptr(), &
    &  lview, lstrides, lextents, lrank, &
    &  gview, gstrides, gextents, grank )
end subroutine GatherScatter__gather_real64_r3_r3

! -----------------------------------------------------------------------------

subroutine GatherScatter__scatter_int32_r1_r1(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in)  :: glb_field_data(:)
  integer(c_int), intent(out) :: loc_field_data(:)
  integer(c_int), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,1) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,1) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  call atlas__GatherScatter__scatter_int( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_int32_r1_r1

subroutine GatherScatter__scatter_int32_r2_r2(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_int), intent(in)  :: glb_field_data(:,:)
  integer(c_int), intent(out) :: loc_field_data(:,:)
  integer(c_int), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__scatter_int( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_int32_r2_r2

subroutine GatherScatter__scatter_int64_r1_r1(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_long), intent(in)  :: glb_field_data(:)
  integer(c_long), intent(out) :: loc_field_data(:)
  integer(c_long), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,1) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,1) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  call atlas__GatherScatter__scatter_long( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_int64_r1_r1

subroutine GatherScatter__scatter_int64_r2_r2(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  integer(c_long), intent(in)  :: glb_field_data(:,:)
  integer(c_long), intent(out) :: loc_field_data(:,:)
  integer(c_long), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__scatter_long( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_int64_r2_r2


subroutine GatherScatter__scatter_real32_r1_r1(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_float), intent(in)  :: glb_field_data(:)
  real(c_float), intent(out) :: loc_field_data(:)
  real(c_float), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,1) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,1) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  call atlas__GatherScatter__scatter_float( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_real32_r1_r1
subroutine GatherScatter__scatter_real32_r2_r2(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_float), intent(in)  :: glb_field_data(:,:)
  real(c_float), intent(out) :: loc_field_data(:,:)
  real(c_float), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__scatter_float( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_real32_r2_r2
subroutine GatherScatter__scatter_real64_r1_r1(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)  :: glb_field_data(:)
  real(c_double), intent(out) :: loc_field_data(:)
  real(c_double), pointer :: lview(:), gview(:)
  integer :: lstrides(1), lextents(1), lrank=1
  integer :: gstrides(1), gextents(1), grank=1
  lstrides = (/ stride(loc_field_data,1) /)
  lextents = (/ 1                        /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,1) /)
  gextents = (/ 1                        /)
  gview => view1d(glb_field_data)
  call atlas__GatherScatter__scatter_double( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_real64_r1_r1
subroutine GatherScatter__scatter_real64_r2_r2(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)  :: glb_field_data(:,:)
  real(c_double), intent(out) :: loc_field_data(:,:)
  real(c_double), pointer :: lview(:), gview(:)
  integer :: lstrides(2), lextents(2), lrank=2
  integer :: gstrides(2), gextents(2), grank=2
  lstrides = (/ stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__scatter_double( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_real64_r2_r2

subroutine GatherScatter__scatter_real64_r3_r3(this, glb_field_data, loc_field_data)
  class(atlas_GatherScatter), intent(in) :: this
  real(c_double), intent(in)  :: glb_field_data(:,:,:)
  real(c_double), intent(out) :: loc_field_data(:,:,:)
  real(c_double), pointer :: lview(:), gview(:)
  integer :: lstrides(3), lextents(3), lrank=3
  integer :: gstrides(3), gextents(3), grank=3
  lstrides = (/ stride(loc_field_data,3), stride(loc_field_data,2), stride(loc_field_data,1) /)
  lextents = (/ 1,                        size  (loc_field_data,2) , size (loc_field_data,1) /)
  lview => view1d(loc_field_data)
  gstrides = (/ stride(glb_field_data,3), stride(glb_field_data,2), stride(glb_field_data,1) /)
  gextents = (/ 1,                        size  (glb_field_data,2) , size (glb_field_data,1) /)
  gview => view1d(glb_field_data)
  if( size(gview) == 0 ) then
    allocate(gview(0))
  endif
  call atlas__GatherScatter__scatter_double( this%c_ptr(), &
    &  gview, gstrides, gextents, grank, &
    &  lview, lstrides, lextents, lrank )
end subroutine GatherScatter__scatter_real64_r3_r3


! -----------------------------------------------------------------------------


