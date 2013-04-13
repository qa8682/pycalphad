/*=============================================================================
	Copyright (c) 2012-2013 Richard Otis

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// declaration for Equilibrium object

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include "libtdb/include/conditions.hpp"
#include "libtdb/include/database.hpp"

/*
 * What this class needs to do:
 * This class will be the foundational piece of how TDBread interacts with
 * libGibbs. Code will construct a Database (using libTDB) and set conditions
 * using an evalconditions object. They will then pass those parameters to an
 * Equilibrium object. The ctor will minimize the Gibbs energy of the system
 * and find the hypersurface corresponding to the n-phase equilibrium.
 * I will move a lot of the evaluate.cpp code into the Equilibrium object.
 * Once minimization is complete, I will have the values of the phase fractions
 * and sublattice site fractions which minimize the Gibbs energy.
 * For convenience in debugging, I will overload an operator to make Equilibrium
 * objects prettyprint in a Thermo-Calc style way.
 * Exception handling will have to be done carefully since a lot of numerical
 * stuff will be happening in the ctor.
 *
 * THOUGHT: Is Boost MultiIndex my salvation here?
 *
 * Equilibrium objects will contain:
 * 1) the name of the database used (for compatibility checking)
 * 2) the value of the minimum Gibbs energy (J/mol)
 * 3) the evalconditions object used for minimization
 * 3) For each phase:
 * 		a) name of phase
 * 		b) fraction of phase
 * 		c) For each sublattice: the stoichiometric coefficient
 * 		d) For each sublattice: the site fraction of each species
 *
 * Equilibrium objects will have:
 * 1) Gibbs minimization for m species and n phases
 * 2) Convenience function for converting site fraction to mole fraction (phase and overall)
 * 3) prettyprint functionality by overloading insertion operator
 * 4) getter functions for the conditions of the equilibrium (should overload evalconditions insertion operator too)
 * 4) (FUTURE) the ability to be constructed from arbitrary (e.g. experimental) data
 */

class Equilibrium {
private:
	const std::string sourcename; // descriptor for the source of the equilibrium data
	const evalconditions conditions; // thermodynamic conditions of the equilibrium
	double mingibbs; // Gibbs energy for the equilibrium (must use common reference state)
	typedef std::unordered_map<std::string, double> speclist; // species name + site fraction
	typedef std::pair<double, speclist> sublattice; // stoichiometric coefficient + species list
	typedef std::vector<sublattice> constitution; // collection of sublattices
	typedef std::pair<double, constitution> phase; // phase fraction + constitution
	typedef std::unordered_map<std::string,phase> phasemap;
	phasemap ph_map; // maps phase name to its object
public:
	Equilibrium(const Database &DB, const evalconditions &conds);
	double mole_fraction(const std::string &specname);
	double mole_fraction(const std::string &specname, const std::string &phasename);
	friend std::ostream& operator<< (std::ostream& stream, const Equilibrium& eq);
};