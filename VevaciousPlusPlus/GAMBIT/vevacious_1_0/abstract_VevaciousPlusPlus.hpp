#ifndef __abstract_VevaciousPlusPlus_vevacious_1_0_hpp__
#define __abstract_VevaciousPlusPlus_vevacious_1_0_hpp__

#include "gambit/Backends/abstractbase.hpp"
#include "forward_decls_abstract_classes.hpp"
#include "forward_decls_wrapper_classes.hpp"
#include <string>
#include <vector>
#include <cstddef>
#include <iostream>

#include "identification.hpp"

namespace CAT_3(BACKENDNAME,_,SAFE_VERSION)
{
    
    
    namespace VevaciousPlusPlus
    {
        class Abstract_VevaciousPlusPlus : public virtual AbstractBase
        {
            public:
    
                virtual void RunPoint(const ::std::basic_string<char, std::char_traits<char>, std::allocator<char> >&) =0;
    
                virtual void ReadLhaBlock(const ::std::basic_string<char, std::char_traits<char>, std::allocator<char> >&, const double, const ::std::vector<std::pair<int, double>, std::allocator<std::pair<int, double> > >&, const int) =0;
    
                virtual void WriteResultsAsXmlFile(const ::std::basic_string<char, std::char_traits<char>, std::allocator<char> >&) =0;
    
                virtual ::std::basic_string<char, std::char_traits<char>, std::allocator<char> > GetResultsAsString() =0;
    
                virtual double GetLifetimeInSeconds() = 0;
                
                virtual void AppendResultsToLhaFile(const ::std::basic_string<char, std::char_traits<char>, std::allocator<char> >&, const bool) =0;
    
                virtual void AppendResultsToLhaFile__BOSS(const ::std::basic_string<char, std::char_traits<char>, std::allocator<char> >&) =0;
    
            public:
                virtual void pointer_assign__BOSS(Abstract_VevaciousPlusPlus*) =0;
                virtual Abstract_VevaciousPlusPlus* pointer_copy__BOSS() =0;
    
            private:
                VevaciousPlusPlus* wptr;
                bool delete_wrapper;
            public:
                VevaciousPlusPlus* get_wptr() { return wptr; }
                void set_wptr(VevaciousPlusPlus* wptr_in) { wptr = wptr_in; }
                bool get_delete_wrapper() { return delete_wrapper; }
                void set_delete_wrapper(bool del_wrp_in) { delete_wrapper = del_wrp_in; }
    
            public:
                Abstract_VevaciousPlusPlus()
                {
                    wptr = 0;
                    delete_wrapper = false;
                }
    
                Abstract_VevaciousPlusPlus(const Abstract_VevaciousPlusPlus&)
                {
                    wptr = 0;
                    delete_wrapper = false;
                }
    
                Abstract_VevaciousPlusPlus& operator=(const Abstract_VevaciousPlusPlus&) { return *this; }
    
                virtual void init_wrapper() =0;
    
                VevaciousPlusPlus* get_init_wptr()
                {
                    init_wrapper();
                    return wptr;
                }
    
                VevaciousPlusPlus& get_init_wref()
                {
                    init_wrapper();
                    return *wptr;
                }
    
                virtual ~Abstract_VevaciousPlusPlus() =0;
        };
    }
    
}


#include "gambit/Backends/backend_undefs.hpp"


#endif /* __abstract_VevaciousPlusPlus_vevacious_1_0_hpp__ */