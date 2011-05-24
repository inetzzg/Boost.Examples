#include "pimpl_example_pointer.h"

// This internal implementation usually only have destructor, constructors,
// data and probably internal methods. Given it is already internal by
// design, it does not need to be a 'class' with "private by default"
// security semantics. All public methods are declared in the external
// visible Test1 class. Then, data in this structure are accessed as
// (*this)->data or (**this).data.
//
// Changes to the contents of this file do not result in consumers to
// recompile (however they will need to relink because the .o file has
// changed).
template<> struct pimpl< my::String >::implementation : boost::noncopyable {
  implementation() : str_(0) { }
  implementation(char const* c_str, std::size_t len) : str_(0) {
    str_ = new std::string(c_str, len);
  }
  ~implementation() {
    if (str_)
      delete str_;
  }

  // Consumers of my Interface are not aware of this or any other member
  // variable.
  std::string* str_;
};


// Begin 'my' namespace here and not above (not sure what the source of the
// breakage is, but it breaks).
namespace my {

// Call the implementation ctor/dtors. Forgetting to do this results in
// linker errors.
String::String() : base() { }
String::String(char const* cstr, std::size_t len) : base(cstr, len) { }


bool String::append(unsigned char byte) {
  (*this)->str_->append(1, byte);
  return true;
}


const std::string* String::cur_str() const { return (*this)->str_; }

} // namespace my
