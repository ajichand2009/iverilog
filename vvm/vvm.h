#ifndef __vvm_H
#define __vvm_H
/*
 * Copyright (c) 1998-2000 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#if !defined(WINNT) && !defined(macintosh)
#ident "$Id: vvm.h,v 1.33 2000/03/16 19:03:04 steve Exp $"
#endif

# include  <cassert>
# include  "vpi_priv.h"

/*
 * The Verilog Virtual Machine are definitions for the virtual machine
 * that executes models that the simulation generator makes.
 */

typedef unsigned vvm_u32;

class vvm_event;
class vvm_thread;
class ostream;


inline vpip_bit_t operator & (vpip_bit_t l, vpip_bit_t r)
{
      if (l == V0) return V0;
      if (r == V0) return V0;
      if ((l == V1) && (r == V1)) return V1;
      return Vx;
}

inline vpip_bit_t operator | (vpip_bit_t l, vpip_bit_t r)
{
      if (l == V1) return V1;
      if (r == V1) return V1;
      if ((l == V0) && (r == V0)) return V0;
      return Vx;
}

inline vpip_bit_t operator ^ (vpip_bit_t l, vpip_bit_t r)
{
      if (l == Vx) return Vx;
      if (l == Vz) return Vx;
      if (r == Vx) return Vx;
      if (r == Vz) return Vx;
      if (l == V0) return r;
      return (r == V0)? V1 : V0;
}

inline vpip_bit_t less_with_cascade(vpip_bit_t l, vpip_bit_t r, vpip_bit_t c)
{
      if (l == Vx) return Vx;
      if (r == Vx) return Vx;
      if (l > r) return V0;
      if (l < r) return V1;
      return c;
}

inline vpip_bit_t greater_with_cascade(vpip_bit_t l, vpip_bit_t r, vpip_bit_t c)
{
      if (l == Vx) return Vx;
      if (r == Vx) return Vx;
      if (l > r) return V1;
      if (l < r) return V0;
      return c;
}

extern vpip_bit_t add_with_carry(vpip_bit_t l, vpip_bit_t r, vpip_bit_t&carry);

inline vpip_bit_t v_not(vpip_bit_t l)
{
      switch (l) {
	  case V0:
	    return V1;
	  case V1:
	    return V0;
	  default:
	    return Vx;
      }
}

extern bool posedge(vpip_bit_t from, vpip_bit_t to);


class vvm_bits_t {
    public:
      virtual ~vvm_bits_t() =0;
      virtual unsigned get_width() const =0;
      virtual vpip_bit_t get_bit(unsigned idx) const =0;

      unsigned as_unsigned() const;
};

extern ostream& operator << (ostream&os, vpip_bit_t);
extern ostream& operator << (ostream&os, const vvm_bits_t&str);

/*
 * Verilog events (update events and nonblocking assign) are derived
 * from this abstract class so that the simulation engine can treat
 * all of them identically.
 */
class vvm_event {

    public:
      vvm_event();
      virtual ~vvm_event() =0;
      virtual void event_function() =0;

      void schedule(unsigned long delay =0);

    private:
      struct vpip_event*event_;

      static void callback_(void*);

    private: // not implemented
      vvm_event(const vvm_event&);
      vvm_event& operator= (const vvm_event&);
};


/*
 * $Log: vvm.h,v $
 * Revision 1.33  2000/03/16 19:03:04  steve
 *  Revise the VVM backend to use nexus objects so that
 *  drivers and resolution functions can be used, and
 *  the t-vvm module doesn't need to write a zillion
 *  output functions.
 *
 * Revision 1.32  2000/03/13 00:02:34  steve
 *  Remove unneeded templates.
 *
 * Revision 1.31  2000/02/23 04:43:43  steve
 *  Some compilers do not accept the not symbol.
 *
 * Revision 1.30  2000/02/23 02:56:56  steve
 *  Macintosh compilers do not support ident.
 *
 * Revision 1.29  2000/01/08 03:09:14  steve
 *  Non-blocking memory writes.
 */
#endif
