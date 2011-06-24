//#ifndef MONOCLE_GUI_BERKELIUM_JSDISPATCH
//#define MONOCLE_GUI_BERKELIUM_JSDISPATCH
//
//#include "JSFunctionBinding.h"
//#include <berkelium/Window.hpp>
//#include <berkelium/ScriptVariant.hpp>
//
//namespace Monocle
//{
//    namespace GUI
//    {
//        namespace JSBinding
//        {
//            #define ALLOW_RETURN_TYPE(type) void Call(JSFunctionBinding<type> *func, ArgList args) { (*func)(args); }
//
//            class JSDispatcher
//            {
//            public:
//                template<typename T>
//                void BindFunction(Berkelium::Window *win, JSFunctionBinding<T>* func)
//                {
//                    std::wcout << "[JSBinding] Binding function: " << func->getName() << std::endl;
//                    if(func == NULL
//                       || win == NULL) return;
//                    //copy the binding so the caller can free the pointer after the call
//                    JSFunctionBinding<T>* localFunc = new JSFunctionBinding<T>(*func);
//
//                    boundFuncs.insert(std::make_pair(localFunc->getName(), localFunc));
//
//                    Berkelium::WideString funcName = Berkelium::WideString::point_to(localFunc->getName().data());
//                    Berkelium::Script::Variant berkFunc = Berkelium::Script::Variant::bindFunction(funcName, false);;
//                    win->addBindOnStartLoading(funcName, berkFunc);
//                }
//
//                void Dispatch(Berkelium::Window* wini,
//                              void *replyMsg,
//                              Berkelium::URLString origin,
//                              Berkelium::WideString funcName,
//                              Berkelium::Script::Variant *args, size_t numArgs);
//
//            private:
//                ALLOW_RETURN_TYPE(void)
//                ALLOW_RETURN_TYPE(std::string)
//                ALLOW_RETURN_TYPE(std::wstring)
//                ALLOW_RETURN_TYPE(char)
//                ALLOW_RETURN_TYPE(int)
//                ALLOW_RETURN_TYPE(long)
//                ALLOW_RETURN_TYPE(double)
//                ALLOW_RETURN_TYPE(float)
//
//                //a map between function names and JSFunctionBinding objects to allow binding of javascript functions.
//                std::map<std::wstring, IFunctionBinding*> boundFuncs;
//            };
//
//        }
//    }
//}
//
//#endif
