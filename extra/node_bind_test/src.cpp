#include <env_node/env_node.hpp>
#include <env/env.hpp>


class addon : public Napi::Addon<addon>
{
public:
    addon(Napi::Env env, Napi::Object exports)
    {
        DefineAddon(
                exports,
                {InstanceValue(
                         "env",
                         Napi::String::New(env, "bindings")),
                 InstanceMethod("test", &addon::test)});
    }

private:
    Napi::Value test(const Napi::CallbackInfo& info)
    {
        if (::env::check())
        {
            return Napi::String::New(info.Env(), "everything is ok!");
        }
        else
        {
            return Napi::String::New(info.Env(), "something is not right :(");
        }
    }
};


NODE_API_NAMED_ADDON(nodeBindTest, addon)
