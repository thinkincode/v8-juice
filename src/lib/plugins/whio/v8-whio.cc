/**
   v8 JS wrappers for the whio i/o device library:

   http://fossil.wanderinghorse.net/repos/whio/

   Author: stephan beal (http://wanderinghorse.net/home/stephan/)

   License: Public Domain

*/
#include <iostream>
#include <sstream>
#include <vector>

/* only for debuggering */
#ifndef CERR
#define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
#endif

#include <v8/juice/forwarding.h>
#include <v8/juice/plugin.h>

#include "v8-whio.h"
#include <v8/juice/ClassBinder.h>


namespace v8 { namespace juice { namespace whio {

    //namespace juice = ::v8::juice;
    using namespace ::v8::juice::convert;

#define JSTR(X) String::New(X)
#define TOSS(X) return ::v8::ThrowException(JSTR(X)) /*juice::ThrowException(X)*/
#define WHIO_RTFM "RTFM: " v8_juice_HOME_PAGE "/wiki/PluginWhio"

    /**
       Static/shared strings, some of which are used as template
       parameters (where we can't use string literals but can,
       curiously enough, use references to pointers to shared
       strings)...
    */
    struct strings
    {
	// Class names:
	static char const * IOBase;
	static char const * IODevice;
	static char const * StreamBase;
	static char const * InStream;
	static char const * OutStream;
	static char const * ByteArray;

	// IOBase:
	static char const * SEEK_CUR_;
	static char const * SEEK_END_;
	static char const * SEEK_SET_;//need suffix b/c SEEK_SET is a #define'd numeric constant
	static char const * canRead;
	static char const * canWrite;
	static char const * close;
	static char const * fileName;
	static char const * flush;
	static char const * isGood;
	static char const * read;
	static char const * toString;
	static char const * write;

	// IODevice:
	static char const * eof;
	static char const * ioDevice;
	static char const * rebound;
	static char const * rewind;
	static char const * seek;
	static char const * size;
	static char const * tell;
	static char const * truncate;
        static char const * clearError;
        static char const * error;

        // InStream:
        static char const * gzip;
        static char const * gunzip;
    };
    char const * strings::IOBase = "IOBase";
    char const * strings::IODevice = "IODevice";
    char const * strings::InStream = "InStream";
    char const * strings::OutStream = "OutStream";
    char const * strings::ByteArray = "ByteArray";
    char const * strings::SEEK_CUR_ = "SEEK_CUR";
    char const * strings::SEEK_END_ = "SEEK_END";
    char const * strings::SEEK_SET_ = "SEEK_SET";
    char const * strings::StreamBase = "StreamBase";
    char const * strings::canRead = "canRead";
    char const * strings::canWrite = "canWrite";
    char const * strings::clearError = "clearError";
    char const * strings::close = "close";
    char const * strings::eof = "eof";
    char const * strings::error = "error";
    char const * strings::fileName = "fileName";
    char const * strings::flush = "flush";
    char const * strings::gzip = "gzipTo";
    char const * strings::gunzip = "gunzipTo";
    char const * strings::ioDevice = "ioDevice";
    char const * strings::isGood = "isGood";
    char const * strings::read = "read";
    char const * strings::rebound = "rebound";
    char const * strings::rewind = "rewind";
    char const * strings::seek = "seek";
    char const * strings::size = "size";
    char const * strings::tell = "tell";
    char const * strings::toString = "toString";
    char const * strings::truncate = "truncate";
    char const * strings::write = "write";

    /**
       Internal binding context for BindNative() and friends.
    */
    //static const void * bind_cx() { return 0;}

    Persistent<Function> IODevice::js_ctor;

    static Handle<Value> abstract_ctor(const Arguments& argv)
    {
	TOSS("This is an abtract class and cannot be instantiated directly!");
    }

    static Handle<Value> abstract_reimplement(const Arguments& argv)
    {
	TOSS("This is an abtract virtual function which is unimplemented by this subclasses!");
    }


// Helper macros for args and type checking:
#define ARGS(COND) const int argc = argv.Length(); if( !(COND) ) TOSS("argument assertion failed: " # COND)
#define DEVH(T,H) T::type * dev = CastFromJS< T >( H )
    //#define DEVH(T,H) WeakJSClassCreator<T>::WrappedType * dev = WeakJSClassCreator<T>::GetSelf( H )
#define DEVHT(T,H) DEVH(T,H); if( ! dev ) TOSS("Native device pointer not found (maybe already closed?)!")
#define DEVHV(T,H) DEVH(T,H); if( ! dev ) return
#define DEVTHIS(T) DEVHT(T,argv.This())

    /**
       whio_dev_subdev_rebound() wrapper. It will only work with
       devices created using whio_dev_subdev_create().
    */
    static Handle<Value> dev_rebound(const Arguments& argv)
    {
	ARGS((argc==2));
	DEVTHIS(IODevice);
	whio_size_t low = CastFromJS<whio_size_t>( argv[0] );
	whio_size_t high = CastFromJS<whio_size_t>( argv[1] );
	return CastToJS( whio_dev_subdev_rebound( dev, low, high ) );
    }

    /**
       Constructor for the native half of IODevice objects.
       
       Curiously, it may not be static because then
       it can't be used as a template type parameter (see
       combined_ctor()). Weird.

       The exception argument should be set to a non-empty value if a
       detailed error message can be reported, otherwise simply return
       0 to indicate failure. If the string is non-empty when this
       function returns, combined_ctor() will throw on our behalf with
       that message (and will destroy the returned object if it is not
       0).

       JS arguments:

       IODevice ctor args:

       - (string filename, bool writeMode)  = file
       - (":memory:" [, int initialSize])   = in-memory buffer
       - (IODevice, int lower, int upper)   = subdevice
     */
    whio_dev * dev_construct( Arguments const & argv,
                              std::string & exception )
    {
	const int argc = argv.Length();
	whio_dev * dev = 0;
	if( 0 == argc )
	{
	    exception = "Not enough arguments for the constructor!";
	}
	HandleScope scope;
	//char const * dontknow = "Don't know how to handle the given constructor arguments!";
        if( (1 == argc) && argv[0]->IsExternal() )
        {
            // we hope it is a whio_dev passed by a "friendly" factory function.
            // fixme: make this a safe operation if someone passes in a different
            // (void *).
            Local< External > ex( External::Cast(*argv[0]) );
            dev = static_cast<whio_dev*>( ex->Value() );
            if( ! dev )
            {
                exception = "Internal ctor got incorrect arguments!";
            }
            return dev;
        }
	Local<Object> self = argv.This();
	if( argv[0]->IsString() )
	{ //ctor(":memory:")
	  //ctor(string filename[, bool writeMode])
	    std::string fname( JSToStdString(argv[0]) );
	    if( ":memory:" == fname )
	    { // (":memory:" [,int size])
		whio_size_t sz = (argc > 1) ? CastFromJS<whio_size_t>(argv[1]) : 0;
		dev = whio_dev_for_membuf( sz, 1.5 );
		if( ! dev )
		{
		    std::ostringstream msg;
		    msg << "Could not create in-memory i/o device of "
			<<sz<<" bytes!";
		    exception = msg.str();
		}
		else
		{
		    self->Set(JSTR(strings::fileName), argv[0], v8::ReadOnly );
		    self->Set(JSTR(strings::canWrite), Boolean::New(true) );
		    self->Set(JSTR(strings::canRead), Boolean::New(true) );
		}
		return dev;
	    }
	    else if( 2 == argc )
	    { // (string filename, bool writeMode)
		bool writeMode = JSToBool(argv[1]);
		char const * mode = (writeMode) ? "r+b" : "rb";
		dev = whio_dev_for_filename( fname.c_str(), mode );
		if( dev )
		{
		    self->Set(JSTR(strings::fileName), argv[0], v8::ReadOnly );
		    self->Set(JSTR(strings::canWrite), Boolean::New(writeMode) );
		    self->Set(JSTR(strings::canRead), Boolean::New(true) );
		}
		return dev;
	    }
	}
	if( (3 == argc) && argv[0]->IsObject() )
	{ // (IODevice,lower,upper)
	    Local<Object> par( Object::Cast(*argv[0]) );
	    whio_dev * iod = CastFromJS<IODevice>(par);
	    if( iod )
	    {
		whio_size_t low = CastFromJS<whio_size_t>(argv[1]);
		whio_size_t high = CastFromJS<whio_size_t>(argv[2]);
		dev = whio_dev_subdev_create( iod, low, high );
		if( ! dev )
		{
		    std::ostringstream msg;
		    msg << "Creation of subdevice for range ("
			<<low<<","<<high<<") failed!";
		    exception = msg.str();
		}
		else
		{
		    self->Set(JSTR(strings::rebound),
			      FunctionTemplate::New(dev_rebound)->GetFunction());
		    self->Set(JSTR(strings::ioDevice), par, v8::ReadOnly);
		    self->Set(JSTR(strings::canWrite), par->Get(JSTR(strings::canWrite)), v8::ReadOnly );
		    self->Set(JSTR(strings::canRead), par->Get(JSTR(strings::canRead)), v8::ReadOnly );
		}
		return dev;
	    }
	}
	exception = "Invalid arguments for IODevice constructor. "
	    WHIO_RTFM;
	return 0;
    }

    whio_dev * ba_construct( Arguments const & argv,
			     std::string & exception )
    {
	const int argc = argv.Length();
	whio_dev * dev = 0;
	HandleScope scope;
	Local<Object> self = argv.This();
	whio_size_t sz = (argc > 0) ? CastFromJS<whio_size_t>(argv[0]) : 0;
	dev = whio_dev_for_membuf( sz, 1.5 );
	if( ! dev )
	{
	    std::ostringstream msg;
	    msg << "Could not create in-memory i/o device of "
		<<sz<<" bytes!";
	    exception = msg.str();
	}
	else
	{
	    self->Set(JSTR(strings::canWrite), Boolean::New(true) );
	    self->Set(JSTR(strings::canRead), Boolean::New(true) );
	}
	return dev;
    }
    /**
       Constructor for the native half of InStream and OutStream
       objects.

       This cannot be static, as explained in dev_construct().

       Note that the writeMode arg is only a template do that we can
       get match the func signature required by combined_ctor().

       Accepted JS args:

       InStream: (writeMode==false)

       - (string filename)

       OutStream (writeMode==true):

       - (string filename [,bool truncate=true])

       Both types:
       - (IODevice)

       omModes:

       - (0)=Out
       - (1)=In
       - (-1)=No-op (throw)
      
    */
    template <int opMode >
    whio_stream * stream_construct(
// 				   Local<Object> self,
// 				    int argc,
// 				    Handle<Value> argv[],
				    Arguments const & argv,
				    std::string & exception )
    {
	if( opMode < 0 )
	{
	    exception = "This is an abstract base type which cannot be directly instantiated!";
	    return 0;
	}
	const int argc = argv.Length();
	if( argc < 1 )
	{
	    exception = "Not enough arguments for the constructor!";
	    return 0;
	}
	HandleScope boo;
	Local<Object> self = argv.This();
	whio_stream * dev = 0;
	const bool writeMode = (0==opMode);
	if( argv[0]->IsString() )
	{// (string filename [, truncate=true])
	    std::string fname = JSToStdString(argv[0]);
	    bool trunc = (argc>1) ? argv[1]->BooleanValue() : true;
	    char const * mode = writeMode ? (trunc ? "wb" : "ab") : "rb";
	    dev = whio_stream_for_filename( fname.c_str(), mode );
	    if(!dev)
	    {
		std::ostringstream msg;
		msg << "Could not open file ["<<fname<<"] using mode '"<<mode<<"'!";
		exception = msg.str();
		return 0;
	    }
	    self->Set(JSTR(strings::fileName), argv[0], v8::ReadOnly );
	    self->Set(JSTR(strings::canWrite), Boolean::New(writeMode), v8::ReadOnly );
	    self->Set(JSTR(strings::canRead), Boolean::New(writeMode ? false : true) );
	    //CERR << "whio_stream_for_filename( "<<fname <<", "<<mode<<" ) == dev@"<<dev<<"\n";
	    return dev;
	}
	whio_dev * iod = CastFromJS<IODevice>( argv[0] );
	if( iod )
	{ // ctor(IODevice)
	    Local<Object> par( Object::Cast(*argv[0]) );
	    if( writeMode )
	    { // try a basic sanity check...
		Local<Value> parRW( par->Get(JSTR(strings::canWrite)) );
		if( writeMode && !parRW->BooleanValue() )
		{
		    std::ostringstream msg;
		    msg << "Cannot create output stream for device because it says it is read-only!";
		    exception = msg.str();
		    return 0;
		}
	    }
	    dev = whio_stream_for_dev( iod, false );
	    if( ! dev )
	    {
		std::ostringstream msg;
		msg << "whio_stream_for_dev( @"<<iod<<", false ) failed!";
		exception = msg.str();
	    }
	    else
	    {
		self->Set(JSTR(strings::ioDevice), par, v8::ReadOnly );
		if( writeMode )
		{
		    self->Set(JSTR(strings::canWrite), par->Get(JSTR(strings::canWrite)), v8::ReadOnly );
		    self->Set(JSTR(strings::canRead), Boolean::New(false) );
		}
		else
		{
		    self->Set(JSTR(strings::canWrite), Boolean::New(false), v8::ReadOnly );
		    self->Set(JSTR(strings::canRead), Boolean::New(true) );
		}
	    }
	    return dev;
	}
	exception = "Invalid arguments for stream constructor. " WHIO_RTFM;
	return 0;
    }


    /**
       write() impl for DevT, which must be one of whio_dev or whio_stream.

       If allowWrite is false then this function throws when called. Set it to
       false for the InStream class.
    */
    template <typename DevT>
    static Handle<Value> devT_write_impl( DevT * dev, const Arguments& argv)
    {
	ARGS((argc==1 || argc==2));
	std::string data( JSToStdString(argv[0]) );
	uint32_t l = (argc>1)
	    ? CastFromJS<uint32_t>( argv[1] )
	    : 0;
	if( 0 == l ) l = data.size();
	if( l > data.size() )
	{
	    return juice::ThrowException("write(input,%u): input data is too short (%u bytes) for write request for %u bytes!",
					 l, data.size(), l);
	}
	return CastToJS( dev->api->write( dev, data.data(), l ) );
    }

    static Handle<Value> stream_write( const Arguments& argv)
    {
	DEVTHIS(OutStream);
	return devT_write_impl<OutStream::type>( dev, argv );
    }
    static Handle<Value> dev_write( const Arguments& argv)
    {
	DEVTHIS(IODevice);
	return devT_write_impl<IODevice::type>( dev, argv );
    }
    /**
       read() impl for DevT, which must be one of whio_dev or whio_stream.
    */
    template <typename DevT>
    static Handle<Value> devT_read_impl( DevT * dev, const Arguments& argv)
    {
	uint32_t l = CastFromJS<uint32_t>( argv[0] );
	if( 0 == l ) TOSS("Number of bytes to read must be greater than 0!");
	std::vector<char> data( l, '\0' );
	whio_size_t rlen = dev->api->read( dev, &data[0], l );
	return String::New( &data[0], static_cast<int>(rlen) );
    }
    static Handle<Value> dev_read(const Arguments& argv)
    {
	ARGS((argc==1));
	DEVTHIS(IODevice);
	return devT_read_impl( dev, argv );
    }

    static Handle<Value> stream_read(const Arguments& argv)
    {
	ARGS((argc==1));
	DEVTHIS(InStream);
	return devT_read_impl( dev, argv );
    }


    /**
       close() impl for whio_dev and whio_stream.
    */
    static Handle<Value> dev_close(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	//CERR << "IODevice.close() == " << dev << '\n';
	WeakJSClassCreator<IODevice>::DestroyObject( argv.This() );
	return Undefined();
    }

    /**
       whio_stream_api::close() wrapper.
    */
    static Handle<Value> stream_close(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(StreamBase);
	//CERR << "StreamBase.close() == " << dev << '\n';
	WeakJSClassCreator<StreamBase>::DestroyObject( argv.This() );
	return Undefined();
    }
    /**
       whio_dev_api::error() wrapper.
    */
    static Handle<Value> dev_error(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return CastToJS( dev->api->error( dev ) );
    }

    /**
       whio_dev_api::clear_error() wrapper.
    */
    static Handle<Value> dev_clear_error(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return CastToJS( dev->api->clear_error( dev ) );
    }

    /**
       whio_dev_api::eof() wrapper.
    */
    static Handle<Value> dev_eof(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return CastToJS( dev->api->eof(dev) );
    }

    /**
       whio_dev_api::tell() wrapper.
    */
    static Handle<Value> dev_tell(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return CastToJS( dev->api->tell( dev ) );
    }

    /**
       whio_dev_api::seek() wrapper.
    */
    static Handle<Value> dev_seek(const Arguments& argv)
    {
	ARGS(((argc==1) || (argc==2)));
	DEVTHIS(IODevice);
	typedef int32_t IntT;
	IntT pos = CastFromJS<IntT>( argv[0] );
	const int whence = (argc>1) ? CastFromJS<IntT>( argv[1] ) : SEEK_SET;
	switch( whence )
	{
	  case SEEK_SET:
	  case SEEK_CUR:
	  case SEEK_END:
	      break;
	  default:
	      TOSS("The second argument to seek() must be one of SEEK_SET, SEEK_CUR, or SEEK_END!");
	      break;
	};
	return CastToJS( dev->api->seek( dev, pos, whence) );
    }

    /**
       flush() impl for DevT, which must be one of whio_dev or whio_stream.
    */
    template <typename DevT>
    static Handle<Value> devT_flush_impl(DevT * dev, const Arguments& argv)
    {
	return CastToJS( dev->api->flush( dev ) );
	// Potential fixme: for subdevices we have no direct (via whio_dev)
	// handle to the parent. If the "parent" is destroyed first then calling
	// flush() on the subdevice (which happens on close()) can crash.
	// In the generic whio API we actually have no way of doing anything about
	// this. Hmmm.
    }

    /**
       whio_dev_api::flush() wrapper.
    */
    static Handle<Value> dev_flush(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return devT_flush_impl<IODevice::type>( dev, argv );
    }
    /**
       whio_stream_api::flush() wrapper.
    */
    static Handle<Value> stream_flush(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(StreamBase);
	return devT_flush_impl<StreamBase::type>( dev, argv );
    }

    /**
       whio_dev_api::truncate() wrapper.
    */
    static Handle<Value> dev_truncate(const Arguments& argv)
    {
	ARGS((argc==1));
	DEVTHIS(IODevice);
	whio_size_t len = CastFromJS<whio_size_t>( argv[0] );
	return CastToJS( dev->api->truncate(dev, len) );
    }

    /**
       whio_dev_size() wrapper.
    */
    static Handle<Value> dev_size(const Arguments& argv)
    {
	ARGS(0==argc);
	DEVTHIS(IODevice);
	return CastToJS( whio_dev_size( dev ) );
    }

    /**
       whio_dev_rewind() wrapper.
    */
    static Handle<Value> dev_rewind(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return CastToJS( whio_dev_rewind( dev ) );
    }

    /** IOBase isGood() impl for whio_dev. */
    static Handle<Value> dev_isgood(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(IODevice);
	return BoolToJS( 0 == dev->api->error(dev) );
    }

    /** IOBase isGood() impl for whio_stream. */
    static Handle<Value> stream_isgood(const Arguments& argv)
    {
	ARGS((0==argc));
	DEVTHIS(StreamBase);
	return BoolToJS( dev->api->isgood(dev) );
    }

#ifndef Z_DEFAULT_COMPRESSION
#define Z_DEFAULT_COMPRESSION (-1)
#endif

    static Handle<Value> stream_gzip(const Arguments& argv)
    {
#if ! WHIO_ENABLE_ZLIB
        TOSS("zlib support was not build in to this library!");
#else
	ARGS((1==argc)||(2==argc));
	DEVTHIS(StreamBase);
        whio_stream * dest = CastFromJS<StreamBase>( argv[0] );
        // FIXME: check for writability on dest and readability on this.
        if( ! dest )
        {
            TOSS("First argument must be an output stream!");
        }
        int lv = (argc>1) ? CastFromJS<int>(argv[1]) : Z_DEFAULT_COMPRESSION;
        int rc = whio_stream_gzip( dev, dest, lv );
        return CastToJS( rc );
#endif
    }

    static Handle<Value> stream_gunzip(const Arguments& argv)
    {
#if ! WHIO_ENABLE_ZLIB
        TOSS("zlib support was not build in to this library!");
#else
	ARGS((1==argc)||(2==argc));
	DEVTHIS(StreamBase);
        whio_stream * dest = CastFromJS<StreamBase>( argv[0] );
        // FIXME: check for writability on dest and readability on this.
        if( ! dest )
        {
            TOSS("First argument must be an output stream!");
        }
        int rc = whio_stream_gunzip( dev, dest );
        return CastToJS( rc );
#endif
    }


    /**
       toString() impl for IOBase family of types. N must
       be the class's name.
    */
    template <char const *&N>
    static Handle<Value> devT_tostring(const Arguments& argv)
    {
	ARGS(0==argc);
	std::ostringstream os;
	Local<Object> self = argv.This();
	os << "[object "<<N;

	Local<String> key(JSTR(strings::fileName));
	if( self->Has(key) )
	{
	    os << ' ' << strings::fileName<<":'"<<JSToStdString(self->Get(key))<<"',";
	}
	key = JSTR(strings::canWrite);
	os << ' ' << strings::canWrite <<':'<< (JSToStdString(self->Get(key))) << ',';
	key = JSTR(strings::canRead);
	os << ' ' << strings::canRead << ':'<< (JSToStdString(self->Get(key)));

	os <<"]";
	return CastToJS( os.str() );
    }


#undef EFSH
#undef EFSHT
#undef EFSTHIS
#undef DEVHT
#undef DEVHV
#undef DEVH
#undef DEVTHIS

    Handle<Value> SetupWhioClasses(const Handle<Object> target )
    {
	HandleScope v8scope;
	Handle<Object> whio = Object::New();
	target->Set(JSTR("whio"),whio);

        {
            Handle<Object> whiorc = Object::New();
            whio->Set(JSTR("rc"),whiorc);

#define SET(K,V) whiorc->Set(JSTR(K), Integer::New(V), v8::ReadOnly ); \
            whiorc->Set(Integer::New(V), JSTR(K), v8::ReadOnly )
            SET("OK", whio_rc.OK );
            SET("ArgError",whio_rc.ArgError);
            SET("IOError",whio_rc.IOError);
            SET("AllocError",whio_rc.AllocError);
            SET("InternalError",whio_rc.InternalError);
            SET("RangeError",whio_rc.RangeError);
            SET("AccessError",whio_rc.AccessError);
            SET("ConsistencyError",whio_rc.ConsistencyError);
            SET("NYIError",whio_rc.NYIError);
            SET("UnsupportedError",whio_rc.UnsupportedError);
            SET("TypeError",whio_rc.TypeError);
            SET("SizeTError",-1);
#undef SET
        }

	////////////////////////////////////////////////////////////
	// IOBase class:
	v8::juice::JSClassCreator bindAbs( strings::IOBase, whio, abstract_ctor );
	Local<Function> func_noop = FunctionTemplate::New(abstract_reimplement)->GetFunction();
	{
	    bindAbs
		.Set(strings::read, func_noop )
		.Set(strings::write ,func_noop)
		.Set(strings::isGood,func_noop)
		.Set(strings::close,func_noop)
		.Set(strings::flush,func_noop)
		.Set(strings::toString, devT_tostring<strings::IOBase> )
		.Set(strings::SEEK_SET_,Integer::New(SEEK_SET) )
		.Set(strings::SEEK_END_, Integer::New(SEEK_END))
		.Set(strings::SEEK_CUR_,Integer::New(SEEK_CUR) )
		.Seal();
	}

	////////////////////////////////////////////////////////////
	// IODevice class:
	WeakJSClassCreator<IODevice> bindIOD( whio );
	{
	    bindIOD
		.Inherit( bindAbs )
 		.Set(strings::write, dev_write )
 		.Set(strings::read, dev_read )
 		.Set(strings::toString, devT_tostring<strings::IODevice> )
 		.Set(strings::flush, dev_flush )
 		.Set(strings::close, dev_close )
		.Set(strings::error,dev_error)
		.Set(strings::clearError,dev_clear_error)
		.Set(strings::eof,dev_eof)
		.Set(strings::tell,dev_tell)
		.Set(strings::seek,dev_seek)
		.Set(strings::truncate,dev_truncate)
		.Set(strings::size,dev_size)
		.Set(strings::rewind,dev_rewind)
		.Set(strings::isGood,dev_isgood)
		.Seal();
            if( IODevice::js_ctor.IsEmpty() )
            {
                IODevice::js_ctor = Persistent<Function>::New( bindIOD.CtorTemplate()->GetFunction() );
            }
        }
	////////////////////////////////////////////////////////////
	// ByteArray class:
	{
	    WeakJSClassCreator<ByteArray>
		bindBA( whio );
            bindBA.Inherit( bindIOD )
 		.Set(strings::toString, devT_tostring<strings::ByteArray> )
		.Seal();
	}

	////////////////////////////////////////////////////////////
	// StreamBase class:
	WeakJSClassCreator<StreamBase> bindSB( whio );
	{
	    bindSB
		.Inherit( bindAbs )
 		.Set(strings::close, stream_close )
 		.Set(strings::flush, stream_flush )
 		.Set(strings::isGood,stream_isgood)
 		.Set(strings::toString, devT_tostring<strings::StreamBase> )
		.Seal();
        }

	////////////////////////////////////////////////////////////
	// InStream class:
	{
	    WeakJSClassCreator<InStream> bindIS( whio );
	    bindIS.Inherit( bindSB )
 		.Set(strings::read, stream_read )
 		.Set(strings::gzip, stream_gzip )
 		.Set(strings::gunzip, stream_gunzip )
 		.Set(strings::toString, devT_tostring<strings::InStream> )
		.Seal();
        }

	////////////////////////////////////////////////////////////
	// OutStream class:
	{
	    WeakJSClassCreator<OutStream> bindOS( whio );
            bindOS.Inherit( bindSB )
 		.Set(strings::write, stream_write )
 		.Set(strings::toString, devT_tostring<strings::OutStream> )
		.Seal();
	}




	return whio;
    }


#undef JSTR
#undef TOSS
#undef ARGS
#undef WHIO_RTFM

    /** Plugin initializer for v8-juice. */
    V8_JUICE_PLUGIN_STATIC_INIT(SetupWhioClasses);

}}} // namespaces