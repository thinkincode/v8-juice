// GENERATED CODE! Edit at your own risk!
/**
    Member function call forwarder for member functions taking 1 arguments.
*/
template < typename WrappedType, typename RV,  typename A0, RV (WrappedType::*Func)( A0)  >
struct MemFuncCallOp1
{
    enum { Arity = 1 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller1 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 1 args and returning void.
*/
template < typename WrappedType,  typename A0, void (WrappedType::*Func)( A0)  >
struct MemFuncCallOp1< WrappedType,void, A0, Func >
{
    enum { Arity = 1 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller1 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 2 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1, RV (WrappedType::*Func)( A0, A1)  >
struct MemFuncCallOp2
{
    enum { Arity = 2 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller2 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 2 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1, void (WrappedType::*Func)( A0, A1)  >
struct MemFuncCallOp2< WrappedType,void, A0, A1, Func >
{
    enum { Arity = 2 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller2 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 3 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2, RV (WrappedType::*Func)( A0, A1, A2)  >
struct MemFuncCallOp3
{
    enum { Arity = 3 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller3 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 3 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2, void (WrappedType::*Func)( A0, A1, A2)  >
struct MemFuncCallOp3< WrappedType,void, A0, A1, A2, Func >
{
    enum { Arity = 3 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller3 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 4 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2,  typename A3, RV (WrappedType::*Func)( A0, A1, A2, A3)  >
struct MemFuncCallOp4
{
    enum { Arity = 4 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller4 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 4 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2,  typename A3, void (WrappedType::*Func)( A0, A1, A2, A3)  >
struct MemFuncCallOp4< WrappedType,void, A0, A1, A2, A3, Func >
{
    enum { Arity = 4 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller4 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 5 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4, RV (WrappedType::*Func)( A0, A1, A2, A3, A4)  >
struct MemFuncCallOp5
{
    enum { Arity = 5 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller5 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 5 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4, void (WrappedType::*Func)( A0, A1, A2, A3, A4)  >
struct MemFuncCallOp5< WrappedType,void, A0, A1, A2, A3, A4, Func >
{
    enum { Arity = 5 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller5 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 6 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5, RV (WrappedType::*Func)( A0, A1, A2, A3, A4, A5)  >
struct MemFuncCallOp6
{
    enum { Arity = 6 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller6 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 6 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5, void (WrappedType::*Func)( A0, A1, A2, A3, A4, A5)  >
struct MemFuncCallOp6< WrappedType,void, A0, A1, A2, A3, A4, A5, Func >
{
    enum { Arity = 6 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller6 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 7 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5,  typename A6, RV (WrappedType::*Func)( A0, A1, A2, A3, A4, A5, A6)  >
struct MemFuncCallOp7
{
    enum { Arity = 7 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller7 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 7 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5,  typename A6, void (WrappedType::*Func)( A0, A1, A2, A3, A4, A5, A6)  >
struct MemFuncCallOp7< WrappedType,void, A0, A1, A2, A3, A4, A5, A6, Func >
{
    enum { Arity = 7 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller7 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Member function call forwarder for member functions taking 8 arguments.
*/
template < typename WrappedType, typename RV,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5,  typename A6,  typename A7, RV (WrappedType::*Func)( A0, A1, A2, A3, A4, A5, A6, A7)  >
struct MemFuncCallOp8
{
    enum { Arity = 8 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller8 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

/**
    Specialization for member functions taking 8 args and returning void.
*/
template < typename WrappedType,  typename A0,  typename A1,  typename A2,  typename A3,  typename A4,  typename A5,  typename A6,  typename A7, void (WrappedType::*Func)( A0, A1, A2, A3, A4, A5, A6, A7)  >
struct MemFuncCallOp8< WrappedType,void, A0, A1, A2, A3, A4, A5, A6, A7, Func >
{
    enum { Arity = 8 };
    typedef WrappedType Type;
    typedef WeakMemFuncCaller8 OpBase;
    static Handle<Value> Call( Type * obj, Arguments const & argv )
    {
	return OpBase::Call( obj, Func, argv );
    }
    static Handle<Value> CallOnWeakSelf( Arguments const & argv )
    {
	return OpBase::CallOnWeakSelf<Type>( Func, argv );
    }
};

