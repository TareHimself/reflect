#include "Test.hpp"

namespace vengine::drawing {

std::shared_ptr<meta::Metadata> DrawingSubsystem::Meta = [](){
return meta::TypeBuilder()
.AddFunction("SubmitThreadSafe",[](const meta::Value& result,const meta::Value& instance, const std::vector<meta::Value>& args){
auto arg_0 = args[0].GetPtr<vk::SubmitInfo2 >(); \
auto arg_1 = args[1].GetPtr< const vk::Fence >(); \
 
instance.GetPtr<DrawingSubsystem>()->SubmitThreadSafe(*arg_0,*arg_1); 
},{})

.Create<DrawingSubsystem>("DrawingSubsystem",{});
}();


std::shared_ptr<meta::Metadata> DrawingSubsystem::GetMeta() const
{
return meta::find<DrawingSubsystem>();
}

}
