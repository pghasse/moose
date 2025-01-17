/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef POROUSFLOWJOINER_H
#define POROUSFLOWJOINER_H

#include "DerivativeMaterialInterface.h"
#include "Material.h"

#include "PorousFlowDictator.h"

//Forward Declarations
class PorousFlowJoiner;

template<>
InputParameters validParams<PorousFlowJoiner>();

/**
 * Material designed to form a std::vector of property
 * and derivatives of these wrt the nonlinear variables
 * from the individual phase properties.
 *
 * Old values are included if include_old=true
 * Values at the quadpoint or the nodes are formed depending on _at_qps
 *
 * Properties can be viscosities, densities, thermal conductivities , etc
 * and the user specifies the property they are interested
 * in using the pf_prop string.
 *
 * Also, using d(property)/dP, d(property)/dS, etc, and
 * dP/dvar, dS/dvar, etc, the matrix of derivatives of property
 * with respect to the nonlinear Variables,  var, are computed.
 *
 * Only values at the nodes are used - not at the quadpoints
 */
class PorousFlowJoiner : public DerivativeMaterialInterface<Material>
{
public:
  PorousFlowJoiner(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties();

  virtual void computeQpProperties();

  /// The variable names UserObject for the Porous-Flow variables
  const PorousFlowDictator & _dictator_UO;

  /// Name of (dummy) pressure variable
  const VariableName _pressure_variable_name;

  /// Name of (dummy) saturation variable
  const VariableName _saturation_variable_name;

  /// Name of (dummy) temperature variable
  const VariableName _temperature_variable_name;

  /// Name of (dummy) mass fraction variable
  const VariableName _mass_fraction_variable_name;

  /// Number of phases
  const unsigned int _num_phases;

  /// Number of porous flow variables
  const unsigned int _num_var;

  /// Name of material property to be joined
  const std::string _pf_prop;

  /// Whether to include old variables
  const bool _include_old;

  /// If _at_qps=true then all quantities are computed at quadpoints, otherwise at the nodes
  const bool _at_qps;

  /// Derivatives of porepressure variable wrt PorousFlow variables at the qps or nodes
  const MaterialProperty<std::vector<std::vector<Real> > > & _dporepressure_dvar;

  /// Derivatives of saturation variable wrt PorousFlow variables at the qps or nodes
  const MaterialProperty<std::vector<std::vector<Real> > > & _dsaturation_dvar;

  /// Derivatives of temperature variable wrt PorousFlow variables at the qps or nodes
  const MaterialProperty<std::vector<std::vector<Real> > > & _dtemperature_dvar;

  /// computed property of the phase
  MaterialProperty<std::vector<Real> > & _property;

  /// old value of property of the phase
  MaterialProperty<std::vector<Real> > * const _property_old;

  /// d(property)/d(PorousFlow variable)
  MaterialProperty<std::vector<std::vector<Real> > > & _dproperty_dvar;

  /// property of each phase
  std::vector<const MaterialProperty<Real> *> _phase_property;

  /// d(property of each phase)/d(pressure)
  std::vector<const MaterialProperty<Real> *> _dphase_property_dp;

  /// d(property of each phase)/d(saturation)
  std::vector<const MaterialProperty<Real> *> _dphase_property_ds;

  /// d(property of each phase)/d(temperature)
  std::vector<const MaterialProperty<Real> *> _dphase_property_dt;
};

#endif //POROUSFLOWJOINER_H
