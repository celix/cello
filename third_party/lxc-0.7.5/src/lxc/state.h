/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2008
 *
 * Authors:
 * Daniel Lezcano <dlezcano at fr.ibm.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _state_h
#define _state_h

typedef enum {
	STOPPED, STARTING, RUNNING, STOPPING,
	ABORTING, FREEZING, FROZEN, THAWED, MAX_STATE,
} lxc_state_t;

extern int lxc_rmstate(const char *name);
extern lxc_state_t lxc_getstate(const char *name);

extern lxc_state_t lxc_str2state(const char *state);
extern const char *lxc_state2str(lxc_state_t state);

#endif
