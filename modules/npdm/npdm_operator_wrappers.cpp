/*                                                                           
Developed by Sandeep Sharma and Garnet K.-L. Chan, 2012                      
Copyright (c) 2012, Garnet K.-L. Chan                                        
                                                                             
This program is integrated in Molpro with the permission of 
Sandeep Sharma and Garnet K.-L. Chan
*/

#include "npdm_patterns.h"
#include "npdm_operator_wrappers.h"

namespace SpinAdapted{

//===========================================================================================================================================================
// 4-INDEX COMPOUND OPERATORS (for one site only)
//===========================================================================================================================================================

Npdm_op_wrapper_compound_CCDD::Npdm_op_wrapper_compound_CCDD( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = 1;
  factor_ = 1.0;
  transpose_ = false;
  build_pattern_ = { '(','(','C','C',')','(','D','D',')',')' };
  // Build singlets only here
  mults_ = { 1, 1 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_compound_CCDD::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx, kx, lx;
  std::vector< boost::shared_ptr<SparseMatrix> > twoOps = spinBlock_->get_op_array(CRE_CRE).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);

  // Assumed single site
  assert ( ix == jx );
  indices_.push_back( ix );
  indices_.push_back( ix );
  indices_.push_back( ix );
  indices_.push_back( ix );

  opReps_.clear();
  // S=0 (+) S=0;  S=0
  opReps_.push_back( build_compound_operator( twoOps, 0, twoOps, 0, 0, ix, true ) );
  // S=1 (+) S=1;  S=0
  opReps_.push_back( build_compound_operator( twoOps, 1, twoOps, 1, 0, ix, true ) );
}

//===========================================================================================================================================================
// 3-INDEX COMPOUND OPERATORS (for one site only)
//===========================================================================================================================================================

Npdm_op_wrapper_compound_CCD::Npdm_op_wrapper_compound_CCD( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = 1;
  factor_ = 1.0;
  transpose_ = false;
  build_pattern_ = { '(','(','C','C',')','D',')' };
  // S={1/2,1/2,3/2}
  mults_ = { 2, 2, 4 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_compound_CCD::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx, kx;

  std::vector< boost::shared_ptr<SparseMatrix> > twoOps = spinBlock_->get_op_array(CRE_CRE).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);
  std::vector< boost::shared_ptr<SparseMatrix> > oneOp = spinBlock_->get_op_array(CRE).get_local_element(idx);
  kx = opReps_.at(0)->get_orbs(0);

  // Assumed single site
  assert ( ix == jx );
  assert ( jx == kx );
  indices_.push_back( ix );
  indices_.push_back( jx );
  indices_.push_back( kx );

  opReps_.clear();
  // S=0 (+) S=1/2;  S=1/2
  opReps_.push_back( build_compound_operator( twoOps, 0, oneOp, 0, 0, ix, true ) );
  // S=1 (+) S=1/2;  S=1/2
  opReps_.push_back( build_compound_operator( twoOps, 1, oneOp, 0, 0, ix, true ) );
  // S=1 (+) S=1/2;  S=3/2
  opReps_.push_back( build_compound_operator( twoOps, 1, oneOp, 1, 0, ix, true ) );
}

//===========================================================================================================================================================
// Actually this whole class is exactly same as CCD case, except for transpose = true, and build_pattern !!

Npdm_op_wrapper_compound_CDD::Npdm_op_wrapper_compound_CDD( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = 1;
  factor_ = 1.0;
  transpose_ = true;
  build_pattern_ = { '(','C','(','D','D',')',')' };
  // S={1/2,1/2,3/2}
  mults_ = { 2, 2, 4 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_compound_CDD::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx, kx;

  std::vector< boost::shared_ptr<SparseMatrix> > twoOps = spinBlock_->get_op_array(CRE_CRE).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);
  std::vector< boost::shared_ptr<SparseMatrix> > oneOp = spinBlock_->get_op_array(CRE).get_local_element(idx);
  kx = opReps_.at(0)->get_orbs(0);

  // Assumed single site
  assert ( ix == jx );
  assert ( jx == kx );
  indices_.push_back( ix );
  indices_.push_back( jx );
  indices_.push_back( kx );

  opReps_.clear();
  // S=0 (+) S=1/2;  S=1/2
  opReps_.push_back( build_compound_operator( twoOps, 0, oneOp, 0, 0, ix, true ) );
  // S=1 (+) S=1/2;  S=1/2
  opReps_.push_back( build_compound_operator( twoOps, 1, oneOp, 0, 0, ix, true ) );
  // S=1 (+) S=1/2;  S=3/2
  opReps_.push_back( build_compound_operator( twoOps, 1, oneOp, 1, 0, ix, true ) );
}

//===========================================================================================================================================================
// 2-INDEX OPERATORS
//===========================================================================================================================================================

Npdm_op_wrapper_CC::Npdm_op_wrapper_CC( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = spinBlock_->get_op_array(CRE_CRE).get_size();
  transpose_ = false;
  build_pattern_ = { '(','C','C',')' };
  // S={0,1}
  mults_ = { 1, 3 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_CC::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx;

  opReps_ = spinBlock_->get_op_array(CRE_CRE).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);

  // Our algorithm assumes 2-particle indices (i,j) s.t. i<=j.  Block stores j<=i, so we commute them (assuming i!=j) and multiply -1
  indices_.push_back( jx );
  indices_.push_back( ix );
  factor_ = 1.0;
  if ( ix != jx ) factor_ = -1.0;
}

//===========================================================================================================================================================

Npdm_op_wrapper_CD::Npdm_op_wrapper_CD( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = spinBlock_->get_op_array(CRE_DES).get_size();
  factor_ = 1.0;
  transpose_ = true;
  build_pattern_ = { '(','C','D',')' };
  // S={0,1}
  mults_ = { 1, 3 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_CD::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx;

  opReps_ = spinBlock_->get_op_array(CRE_DES).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);

  // Our algorithm assumes 2-particle indices (i,j) s.t. i<=j.  Block stores j<=i, but the transpose takes care of it.
  indices_.push_back( jx );
  indices_.push_back( ix );
}

//===========================================================================================================================================================

Npdm_op_wrapper_DD::Npdm_op_wrapper_DD( SpinBlock * spinBlock )
{
  spinBlock_ = spinBlock;
  size_ = spinBlock_->get_op_array(CRE_CRE).get_size();
  factor_ = 1.0;
  transpose_ = true;
  build_pattern_ = { '(','D','D',')' };
  // S={0,1}
  mults_ = { 1, 3 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_DD::set_local_ops( int idx )
{
  // Spatial orbital indices
  indices_.clear();
  int ix, jx;

  opReps_ = spinBlock_->get_op_array(CRE_CRE).get_local_element(idx);
  ix = opReps_.at(0)->get_orbs(0);
  jx = opReps_.at(0)->get_orbs(1);

  // Our algorithm assumes 2-particle indices (i,j) s.t. i<=j.  Block stores j<=i, but the transpose takes care of it.
  indices_.push_back( jx );
  indices_.push_back( ix );
}

//===========================================================================================================================================================
// 1-INDEX OPERATORS
//===========================================================================================================================================================

Npdm_op_wrapper_C::Npdm_op_wrapper_C( SpinBlock * spinBlock ) 
{
  spinBlock_ = spinBlock;
  size_ = spinBlock_->get_op_array(CRE).get_size();
  factor_ = 1.0;
  transpose_ = false;
  build_pattern_ = { '(','C',')' };
  // S=1/2 only
  mults_ = { 2 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_C::set_local_ops( int idx )
{
  indices_.clear();
  opReps_ = spinBlock_->get_op_array(CRE).get_local_element(idx);
  int ix = opReps_.at(0)->get_orbs(0);
  indices_.push_back(ix);
}

//===========================================================================================================================================================

Npdm_op_wrapper_D::Npdm_op_wrapper_D( SpinBlock * spinBlock ) 
{
  spinBlock_ = spinBlock;
  size_ = spinBlock_->get_op_array(CRE).get_size();
  factor_ = 1.0;
  transpose_ = true;
  build_pattern_ = { '(','D',')' };
  // S=1/2 only
  mults_ = { 2 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
// FIXME (check how to use initialization lists for base classes...??)
void Npdm_op_wrapper_D::set_local_ops( int idx )
{
  indices_.clear();
  opReps_ = spinBlock_->get_op_array(CRE).get_local_element(idx);
  int ix = opReps_.at(0)->get_orbs(0);
  indices_.push_back(ix);
}

//===========================================================================================================================================================

Npdm_op_wrapper_NULL::Npdm_op_wrapper_NULL()
{
  // Null operator
  size_ = 0;
  factor_ = 1.0;
  transpose_ = false;
  build_pattern_ = { '(',')' };
  mults_ = { 0 };
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void Npdm_op_wrapper_NULL::set_local_ops( int idx )
{
  // Null operator
  indices_.clear();
  opReps_.clear();
  opReps_.reserve(1); // Just for compatibility
  indices_.push_back(-1);
}


//===========================================================================================================================================================
// NOTE transpose applied to RHS operator here

boost::shared_ptr<SparseMatrix> NpdmSpinOps::build_compound_operator( std::vector< boost::shared_ptr<SparseMatrix> > lhsOps, int ilhs,
                                                                      std::vector< boost::shared_ptr<SparseMatrix> > rhsOps, int irhs,
                                                                      int ispin, int ix, bool transpose )
{
  // Initialize new operator
  boost::shared_ptr<SparseMatrix> newOp (new Cre);
  newOp->set_orbs() = rhsOps.at(0)->get_orbs();
  newOp->set_orbs().push_back(ix); 
  newOp->set_orbs().push_back(ix);
  newOp->set_initialised() = true;
  newOp->set_fermion() = false;
  newOp->allocate( spinBlock_->get_stateInfo() );

  if (transpose) {
    // Build compound operator as product of LHS and TRANSPOSE( RHS )
    newOp->set_deltaQuantum() = ( lhsOps.at(ilhs)->get_deltaQuantum() - rhsOps.at(irhs)->get_deltaQuantum() ).at(ispin);
    operatorfunctions::Product(spinBlock_, *(lhsOps.at(ilhs)), Transposeview(*rhsOps.at(irhs)), *newOp, 1.0 );
  }
  else {
    // Build compound operator as product of LHS and RHS
    newOp->set_deltaQuantum() = ( lhsOps.at(ilhs)->get_deltaQuantum() - rhsOps.at(irhs)->get_deltaQuantum() ).at(ispin);
    operatorfunctions::Product(spinBlock_, *(lhsOps.at(ilhs)), *(rhsOps.at(irhs)), *newOp, 1.0 );
  }

  return newOp;
}

//===========================================================================================================================================================

NpdmSpinOps init_npdm_operators( SpinBlock * spinBlock, std::vector<Npdm::CD> cd_type )
{
  std::vector<Npdm::CD> op;

  // 0-index operators
  if ( cd_type.size() == 0 ) return Npdm_op_wrapper_NULL();

  // 1-index operators
  op = { Npdm::CREATION };
  if ( cd_type == op ) return Npdm_op_wrapper_C( spinBlock );
  op = { Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_D( spinBlock );

  // 2-index operators
  op = { Npdm::CREATION, Npdm::CREATION };
  if ( cd_type == op ) return Npdm_op_wrapper_CC( spinBlock );
  op = { Npdm::CREATION, Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_CD( spinBlock );
  op = { Npdm::DESTRUCTION, Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_DD( spinBlock );

  // 3-index (dot) operators
  op = { Npdm::CREATION, Npdm::CREATION, Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_compound_CCD( spinBlock );
  op = { Npdm::CREATION, Npdm::DESTRUCTION, Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_compound_CDD( spinBlock );

  // 4-index (dot) operators
  op = { Npdm::CREATION, Npdm::CREATION, Npdm::DESTRUCTION, Npdm::DESTRUCTION };
  if ( cd_type == op ) return Npdm_op_wrapper_compound_CCDD( spinBlock );

  assert (false );
}

//===========================================================================================================================================================

}
