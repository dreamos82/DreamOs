AC_INIT(DreamOS, 0.3-trunk)

AC_LANG(C)
AC_PROG_CC

dnl super: if present 'sudo', if not 'su', else blank
AC_CHECK_PROG(super, sudo, sudo bash -c)
AC_CHECK_PROG(super, su, su -c)

AC_SUBST(SU, ["$super"])

AC_OUTPUT(Makefile)
