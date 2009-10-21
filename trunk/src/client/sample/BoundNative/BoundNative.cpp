// EXPERIMENTAL/INCOMPLETE!
#include <sstream>

#include "ClassWrap.h"


#include <unistd.h> // sleep(3)

#ifndef CERR
#include <iostream> /* only for debuggering */
#define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
#endif

namespace v8 { namespace juice {
    /** A class for testing ClasWrap. */
    struct BoundNative
    {
    private:
        int propi;
        static size_t instcount;
    public:
        BoundNative()
            : propi(13)
        {
            ++instcount;
        }
        ~BoundNative()
        {
            --instcount;
        }
        bool ptr( BoundNative const * b )
        {
            CERR << "BoundNative[@"<<(void const *)this<<"]->ptr("<<(void const *)b<<")\n";
            return 0 != b;
        }
        static size_t InstanceCount()
        {
            return instcount;
        }
        int getInt() const { return this->propi; }
        void setInt( int i ) { this->propi = i; }
        std::string toString() const
        {
            std::ostringstream os;
            os << "[Object BoundNative@"<<(void const *)this<<']';
            return os.str();
        }
        void tryRoundaboutApproach()
        {
            CERR << "BoundNative[@"<<(void const *)this<<"]::tryRoundaboutApproach()\n";
        }
        Handle<Value> toString2( Arguments const & argv )
        {
#if 0
            typedef convert::MemFuncForwarder<0> MF;
            return MF::Call( this, &BoundNative::tryRoundaboutApproach, argv );
#else
            return convert::CastToJS( this->toString() );
#endif
        }
    };
    size_t BoundNative::instcount = 0;

    template <>
    struct ClassWrap_Ops_ClassName<BoundNative>
    {
        static char const * Value()
        {
            return "BoundNative";
        }
    };

    template <>
    struct ClassWrap_Opt_InternalFields<BoundNative> : ClassWrap_Opt_Int<3>
    {
        static const int NativeIndex = 2;
    };

    template <>
    struct ClassWrap_Ops_Memory<BoundNative>
    {
        typedef ClassWrap_Types<BoundNative>::Type Type;
        typedef ClassWrap_Types<BoundNative>::NativeHandle NativeHandle;
	static NativeHandle Instantiate( Arguments const &  /*argv*/,
                                         std::ostream & /* exceptionText */ )
	{
            NativeHandle x = new BoundNative;
            CERR << "BoundNative->Instantiate() == @"<<(void const *)x<<'\n';
            v8::V8::AdjustAmountOfExternalAllocatedMemory( sizeof(BoundNative) );
            return x;
	}
	static void Destruct( NativeHandle obj )
	{
            CERR << "BoundNative->Destruct() == @"<<(void const *)obj<<'\n';
            v8::V8::AdjustAmountOfExternalAllocatedMemory( -sizeof(BoundNative) );
            delete static_cast<NativeHandle>(obj);
	}
    };

}} // namespaces

#define CLASSWRAP_BOUND_TYPE v8::juice::BoundNative
#include "ClassWrap-CastOps.h"

namespace v8 { namespace juice {
    std::string BoundNative_version()
    {
        return "alphalpha 0.0.1";
    }
    void BoundNative_doSomething(std::string const &x)
    {
        CERR << "doSomething(\""<<x<<"\")\n";
    }
    size_t BoundNative_doSomething2(std::string const &x)
    {
        CERR << "doSomething2(\""<<x<<"\")\n";
        return 42;
    }
    void bind_my_native( v8::Handle<v8::Object> dest )
    {
        using namespace v8;
        using namespace v8::juice;
        HandleScope scope;
        typedef BoundNative N;
        typedef ClassWrap<N> CW;
        CW cw(dest);
        CERR <<"Binding class "<<CW::ClassName::Value()<<"...\n";
        cw.Set("foo",String::New("this is foo"));
        cw.Set("toString2", convert::InvocationCallbackMember<N,&N::toString2>::Call );
        //cw.BindMemberFunc<&N::toString2>("toString");
        typedef
            convert::InvocationCallbackCreator
            //convert::FunctionForwarder<0>
            ICC;
        cw.Set( "toString",
                ICC::M0::Invocable<N,std::string,&N::toString>
                );
        cw.Set( "getInt",
                ICC::M0::Invocable<N,int,&N::getInt>
                );
        cw.Set( "setInt",
                ICC::M1::Invocable<N,void,int,&N::setInt>
                );
        cw.Set( "ptr",
                ICC::M1::Invocable<N,bool,N const * ,&N::ptr>
                //ICC::M1::InvocableVoid<N,bool,N const * ,&N::ptr>
                );
        v8::InvocationCallback FH;
        FH =
            ICC::F0::Invocable<std::string,BoundNative_version>
            //ICC::Invocable<std::string,BoundNative_version>
            ;
#define JFH v8::FunctionTemplate::New(FH)->GetFunction()
        cw.Set( "version", JFH );

        FH = ICC::F1::Invocable<void,std::string const &,BoundNative_doSomething>;
        FH = ICC::F1::InvocableVoid<size_t,std::string const &,BoundNative_doSomething2>;
        FH = ICC::F1::Invocable<size_t,std::string const &,BoundNative_doSomething2>;

        cw.Set( "doSomething", JFH );
        FH = ICC::F1::Invocable<unsigned int,unsigned int,::sleep>;
        cw.Set( "sleep", JFH );
#undef JFH
        cw.Seal();
        v8::HandleScope hscope;
        Handle<Object> jobj = cw.NewInstance(0,0);
        N * bound = CW::ToNative::Value(jobj);
        CERR << "bound == @"<<(void const *)bound<<'\n';
        CERR << "bound == @"<<convert::CastFromJS<N>( jobj )<<'\n';
        if( bound )
        {
            typedef convert::MemFuncForwarder<0> MFF;
            //Handle<Value> = MFF::Call( obj, BoundNative::toString
        }
#if 0
        if(0 && bound)
        {
            Handle<Object> j2 = CW::CastToJS::Value( bound );
            CERR << "JW::CastToJS::Value(jobj) == "<<convert::CastFromJS<std::string>(j2)<<'\n';
        }
#endif
        CERR <<"Binding done.\n";
    }

} } // namespaces

