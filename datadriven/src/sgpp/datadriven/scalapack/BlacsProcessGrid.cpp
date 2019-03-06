/*
 * Copyright (C) 2008-today The SG++ project
 * This file is part of the SG++ project. For conditions of distribution and
 * use, please see the copyright notice provided with SG++ or at
 * sgpp.sparsegrids.org
 *
 * BlacsProcessGrid.cpp
 *
 * Created on: Jan 14, 2019
 *     Author: Jan Schopohl
 */
#ifdef USE_SCALAPACK

#include <sgpp/datadriven/scalapack/BlacsProcessGrid.hpp>

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <sgpp/datadriven/scalapack/blacs.hpp>
#include <sgpp/datadriven/scalapack/scalapack.hpp>

#include <unistd.h>

namespace sgpp {
namespace datadriven {

int BlacsProcessGrid::systemContext = 0;
int BlacsProcessGrid::numberOfProcesses = 0;
bool BlacsProcessGrid::blacsInitialized = false;

BlacsProcessGrid::BlacsProcessGrid() {
  if (!blacsInitialized) {
    // TODO(jan) exception
  }
  int rows = static_cast<int>(std::sqrt(numberOfProcesses));
  int columns = static_cast<int>(std::sqrt(numberOfProcesses));
  BlacsProcessGrid(rows, columns);
}

BlacsProcessGrid::BlacsProcessGrid(int rows, int columns) : rows(rows), columns(columns) {
  if (!blacsInitialized) {
    // TODO(jan) exception
  }

  int ignore = -1;
  int systemContext = -1;
  Cblacs_get(ignore, 0, systemContext);
  ictxt = systemContext;
  Cblacs_gridinit(ictxt, "R", this->rows, this->columns);

  // TODO(jan) error handling
  Cblacs_gridinfo(ictxt, this->rows, this->columns, myrow, mycolumn);

  if (myrow >= 0 && mycolumn >= 0) {
    this->mypnum = Cblacs_pnum(ictxt, myrow, mycolumn);
    this->partOfGrid = true;
  } else {
    this->mypnum = -1;
    this->partOfGrid = false;
  }
}

BlacsProcessGrid::~BlacsProcessGrid() {
  if (this->mypnum >= 0) {
    // only exit the grid if this process is actually part of it
    Cblacs_gridexit(ictxt);
  }
}

int BlacsProcessGrid::getContextHandle() const { return this->ictxt; }

int BlacsProcessGrid::getTotalRows() const { return this->rows; }

int BlacsProcessGrid::getTotalColumns() const { return this->columns; }

int BlacsProcessGrid::getCurrentRow() const { return this->myrow; }

int BlacsProcessGrid::getCurrentColumn() const { return this->mycolumn; }

int BlacsProcessGrid::getCurrentProcess() const { return this->mypnum; }

bool BlacsProcessGrid::isProcessInGrid() const { return this->partOfGrid; }

size_t BlacsProcessGrid::availableProcesses() {
  if (!blacsInitialized) {
    // TODO(jan) exception
  }
  return static_cast<size_t>(numberOfProcesses);
}

void BlacsProcessGrid::initializeBlacs() {
  // init BLACS and the MPI environment
  std::cout << "Init BLACS and MPI" << std::endl;
  MPI_Init(nullptr, nullptr);
  int mypnum;
  Cblacs_pinfo(mypnum, numberOfProcesses);
}

void BlacsProcessGrid::exitBlacs() {
  // Finalizes ScaLAPACK, BLACS and the MPI environment
  std::cout << "Exit BLACS and MPI" << std::endl;
  Cblacs_exit(1);
  MPI_Finalize();
}

}  // namespace datadriven
}  // namespace sgpp

#endif  // USE_SCALAPACK