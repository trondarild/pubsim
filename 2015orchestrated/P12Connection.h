/* 
* Copyright 2014 Friedemann Zenke
*
* This file is part of Auryn, a simulation package for plastic
* spiking neural networks.
* 
* Auryn is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Auryn is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Auryn.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef P12CONNECTION_H_
#define P12CONNECTION_H_

#include "auryn_definitions.h"
#include "DuplexConnection.h"
#include "EulerTrace.h"
#include "LinearTrace.h"

#define P12DECAYTERM_H 4.0
#define P12CONNECTION_EULERUPGRADE_STEP 0.99

using namespace std;


class P12Connection : public DuplexConnection
{

private:
	// STP parameters (maybe this should all move to a container)
	auryn_vector_float * state_x;
	auryn_vector_float * state_u;
	auryn_vector_float * state_temp;


	EulerTrace * mem_lp;


	double tau_d;
	double tau_f;
	double Urest;
	double Ujump;


	auryn_vector_float * hom;

	AurynFloat tau_plus;
	AurynFloat tau_minus;
	AurynFloat tau_long;
	NeuronID * fwd_ind; 
	AurynWeight * fwd_data;

	NeuronID * bkw_ind; 
	AurynWeight ** bkw_data;

	AurynFloat hom_target; 
	AurynFloat hom_fudge; 
	AurynFloat beta; 
	AurynFloat beta_fudge;

	AurynWeight weight_a;
	AurynWeight weight_b;
	AurynWeight weight_c;

	// Homeostasis parameters
	AurynFloat tau_hom;
	AurynDouble delta_hom;

	AurynFloat tau_consolidation;
	AurynFloat delta_consolidation;
	AurynTime timestep_consolidation;
	ForwardMatrix * w_solid_matrix;

	void push_attributes();

	void init_shortcuts();


public:
	AurynFloat A3_plus;
	AurynFloat A2_plus;
	AurynFloat A2_minus;
	AurynFloat delta;
	AurynFloat pot_strength;

	PRE_TRACE_MODEL * tr_pre;
	DEFAULT_TRACE_MODEL * tr_post;
	DEFAULT_TRACE_MODEL * tr_post2;

	inline AurynWeight dw_pre(const NeuronID post, const AurynWeight * w);
	inline AurynWeight dw_post(const NeuronID pre, const NeuronID post, const AurynWeight * w, const AurynWeight w0);

	void propagate_forward();
	void propagate_backward();

	bool stdp_active;
	bool consolidation_active;

	P12Connection(SpikingGroup * source, NeuronGroup * destination, TransmitterType transmitter=GLUT);

	P12Connection(SpikingGroup * source, NeuronGroup * destination, 
			const char * filename, 
			AurynFloat eta=1, 
			AurynFloat kappa=3., AurynFloat maxweight=100. , 
			TransmitterType transmitter=GLUT);

	P12Connection(SpikingGroup * source, NeuronGroup * destination, 
			AurynWeight weight, AurynFloat sparseness=0.05, 
			AurynFloat eta=1, 
			AurynFloat kappa=3., AurynFloat maxweight=100. , 
			TransmitterType transmitter=GLUT,
			string name = "P12Connection" );

	virtual ~P12Connection();
	void init(AurynFloat eta, AurynFloat kappa, AurynFloat maxweight);

	void set_tau_hom(AurynFloat tau);

	void set_beta(AurynFloat b);
	void set_tau_d(AurynFloat taud);
	void set_tau_f(AurynFloat tauf);
	void set_ujump(AurynFloat r);
	void set_urest(AurynFloat r);

	void consolidate();

	void set_weight_a(AurynFloat w0);
	void set_weight_c(AurynFloat w2);
	void free();

	virtual void propagate();
	virtual void evolve();

	virtual void finalize();

	void load_fragile_matrix(string filename);

	virtual bool load_from_file(string filename);
	virtual bool write_to_file(string filename);

};

#endif /*P12CONNECTION_H_*/
