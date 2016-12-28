#include "declaration.h"

#include <datstr/intrusiveptrimpl.h>

template class cIntrusivePtrFull< ::OTest2::Declaration >;

namespace OTest2 {

Declaration::Declaration() {

}

Declaration::~Declaration() {

}

void Declaration::incRef() {
  refcount.incRef();
}

void Declaration::decRef() {
  if(!refcount.decRef())
    delete this;
}

} /* -- namespace OTest2 */
