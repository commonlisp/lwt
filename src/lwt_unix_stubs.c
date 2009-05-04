/* Lightweight thread library for Objective Caml
 * http://www.ocsigen.org/lwt
 * Module Lwt_unix_stubs
 * Copyright (C) 2009 Jérémie Dimino
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, with linking exceptions;
 * either version 2.1 of the License, or (at your option) any later
 * version. See COPYING file for details.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/unixsupport.h>

/* +------------+
   | Read/write |
   +------------+ */

/* This code is a simplified version of the default unix_write and
   unix_read functions of caml.

   Since we know that reading or writing will never block we can
   directly use the buffer from the managed memory without copying it
   (thus removing the limitation of 16KB by operation).
*/

CAMLprim value lwt_unix_write(value fd, value buf, value ofs, value len)
{
  CAMLparam4(fd, buf, ofs, len);
  int ret;
  ret = write(Int_val(fd), &Byte(String_val(buf), Long_val(ofs)), Long_val(len));
  if (ret == -1) uerror("lwt_unix_write", Nothing);
  CAMLreturn(Val_int(ret));
}

CAMLprim value lwt_unix_read(value fd, value buf, value ofs, value len)
{
  CAMLparam4(fd, buf, ofs, len);
  int ret;
  ret = read(Int_val(fd), &Byte(String_val(buf), Long_val(ofs)), Long_val(len));
  if (ret == -1) uerror("lwt_unix_read", Nothing);
  CAMLreturn(Val_int(ret));
}

/* +----------------+
   | Terminal sizes |
   +----------------+ */

#include <sys/ioctl.h>
#include <termios.h>

CAMLprim value lwt_unix_term_size(value fd) {
  CAMLparam1(fd);

  struct winsize size;
  if (ioctl(Int_val(fd), TIOCGWINSZ, &size) < 0)
    caml_failwith("ioctl(TIOCGWINSZ)");

  value result = caml_alloc_tuple(2);
  Field(result, 0) = Val_int(size.ws_row);
  Field(result, 1) = Val_int(size.ws_col);
  CAMLreturn(result);
}
