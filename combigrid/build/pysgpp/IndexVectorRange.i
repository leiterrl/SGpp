// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

// %include "combigrid/src/sgpp/combigrid/tools/IndexVectorRange.hpp"
// make IndexVectorRange iterable from Python
// https://stackoverflow.com/questions/35291053/how-to-make-a-c-class-iterable-from-python-using-swig
%inline %{
namespace sgpp{
namespace combigrid{
class StopIterator {};
}
}
%}

%{
#include <sgpp/combigrid/tools/IndexVectorIterator.hpp>
#include <sgpp/combigrid/tools/IndexVectorRange.hpp>
%}

%include "combigrid/src/sgpp/combigrid/tools/IndexVectorIterator.hpp"
%include "combigrid/src/sgpp/combigrid/tools/IndexVectorRange.hpp"

%include "exception.i"
namespace sgpp {
  namespace combigrid {
    %exception IndexVectorIterator::__next__ {
      try
      {
        $action // calls %extend function next() below
      }
      catch (sgpp::combigrid::StopIterator)
      {
        PyErr_SetString(PyExc_StopIteration, "End of iterator");
        return NULL;
      }
    };

    %extend IndexVectorIterator
    {
      IndexVectorIterator*__iter__()
      {
        return $self;
      }

      IndexVector& __next__()
      {
        if (!($self->isAtEnd()))
        {
          // dereference the iterator and return reference to the object,
          // then increment
          return *($self->operator++());
        }
        throw sgpp::combigrid::StopIterator();
      }

      IndexVectorIterator* increment()
      {
        if (!($self->isAtEnd()))
        {
          // dereference the iterator and return reference to the object,
          // then increment
          $self->operator++();
          return $self;
        }
        throw sgpp::combigrid::StopIterator();
      }
    };

    %extend IndexVectorRange {
      IndexVectorIterator __iter__()
      {
        return $self->begin();
      }
    };
  }
}
