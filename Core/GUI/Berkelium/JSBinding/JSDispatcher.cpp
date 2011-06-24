//#include "JSDispatcher.h"
//
//#include <Monocle.h>
//
//#include <berkelium/Berkelium.hpp>
//#include <berkelium/Window.hpp>
//#include <berkelium/ScriptVariant.hpp>
//
//#include <boost/any.hpp>
//
//namespace Monocle
//{
//    namespace GUI
//    {
//        namespace JSBinding
//        {
//            #define CALL_IF_TYPE(type) if(func->GetReturnType() == typeid(type)) { Call((JSFunctionBinding<type> *)func, inArgs); }
//
//            void JSDispatcher::Dispatch(Berkelium::Window* wini,
//                          void *replyMsg,
//                          Berkelium::URLString origin,
//                          Berkelium::WideString funcName,
//                          Berkelium::Script::Variant *args, size_t numArgs)
//            {
//                std::map<std::wstring, IFunctionBinding *>::iterator it;
//                std::wcout << funcName.data() << std::endl;
//
//                it = boundFuncs.find((std::wstring)funcName.data());
//
//                if(it != boundFuncs.end())
//                {
//                    IFunctionBinding *func = (*it).second;
//
//                    ArgList inArgs;
//
//                    for(int i = 0; i < numArgs; i++)
//                    {
//                        Berkelium::Script::Variant::Type type = args[i].type();
//                        if(type == Berkelium::Script::Variant::JSSTRING)
//                            inArgs.push_back(std::make_pair(L"" + i, boost::any(args[i].toString().data())));
//                        else if(type == Berkelium::Script::Variant::JSDOUBLE)
//                            inArgs.push_back(std::make_pair(L"" + i, boost::any(args[i].toDouble())));
//                        else if(type == Berkelium::Script::Variant::JSBOOLEAN)
//                            inArgs.push_back(std::make_pair(L"" + i, boost::any(args[i].toBoolean())));
//                        else
//                            //don't know how to handle type
//                            inArgs.push_back(std::make_pair(L"" + i, boost::any(args[i])));
//
//                    }
//
//                    CALL_IF_TYPE(void)
//                    else CALL_IF_TYPE(std::string)
//                    else CALL_IF_TYPE(std::wstring)
//                    else CALL_IF_TYPE(char)
//                    else CALL_IF_TYPE(int)
//                    else CALL_IF_TYPE(long)
//                    else CALL_IF_TYPE(double)
//                    else CALL_IF_TYPE(float)
//
//                    if(replyMsg) wini->synchronousScriptReturn(replyMsg, Berkelium::Script::Variant());
//                }
//                else
//                {
//                    //didn't find function
//                    if(replyMsg)
//                    {
//                        //in case JS expects return value, return default.
//                        wini->synchronousScriptReturn(replyMsg, Berkelium::Script::Variant());
//                    }
//                }
//            }
//        }
//    }
//}
//
//
