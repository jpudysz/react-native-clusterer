#include <jsi/jsi.h>
#include "react-native-clusterer.h"
#include "clusterer.h"

using namespace facebook;
using namespace std;

namespace clusterer
{

	void install(jsi::Runtime &jsiRuntime)
	{
		auto init = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "init"), 2, [](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
														  {
				if (count != 3)
				{
				  jsi::detail::throwJSError(rt, "React-Native-Clusterer: init expects 3 arguments");
				  return jsi::Value();
				}

				if(!args[0].isString())
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: init expects a string as third argument");
					return jsi::Value();
				}

				cluster_init(rt, args[0], args[1], args[2]);
				return jsi::Value();
				});

		auto getTile = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "getTile"), 4, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
															 {
				if (count != 4)
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getTile expects 4 arguments");
					return jsi::Value();
				}

				if (!args[0].isString() || !args[1].isNumber() || !args[2].isNumber() || !args[3].isNumber())
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getTile expects string and 3 numbers as arguments");
					return jsi::Value();
				}

				return  cluster_getTile(rt, args[0].asString(rt).utf8(rt), (int)args[1].asNumber(), (int)args[2].asNumber(), (int)args[3].asNumber());
				});

		auto getClusters = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "getClusters"), 3, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
															 {
				if (count != 3)
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getClusters expects 2 arguments");
					return jsi::Value();
				}

				if (!args[0].isString() || !args[1].asObject(rt).isArray(rt) || !args[2].isNumber())
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getClusters expects an array and a number");
					return jsi::Value();
				}
				try {
					auto jsibbox = args[1].asObject(rt).asArray(rt);
					double bbox[4] = {
							jsibbox.getValueAtIndex(rt, 0).asNumber(),
							jsibbox.getValueAtIndex(rt, 1).asNumber(),
							jsibbox.getValueAtIndex(rt, 2).asNumber(),
							jsibbox.getValueAtIndex(rt, 3).asNumber(),
					};
					return  cluster_getClusters(rt, args[0].asString(rt).utf8(rt), bbox, (int)args[2].asNumber());
				} catch (const std::exception& e) {
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: GetClusters error, make sure boundingBox is an array of 4 numbers");
                    return jsi::Value();
				}});


		auto getChildren = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "getChildren"), 2, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
																 {
				if (count != 2)
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getTile expects 2 arguments");
					return jsi::Value();
				}

				if (!args[0].isString() || !args[1].isNumber() )
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getTile expects string and a number as arguments");
					return jsi::Value();
				}

				return cluster_getChildren(rt, args[0].asString(rt).utf8(rt), (int)args[1].asNumber());
				});

		auto getLeaves = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "getLeaves"), 4, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
															   {
				if (count != 4)
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getLeaves expects 4 arguments");
					return jsi::Value();
				}
					if (!args[0].isString() || !args[1].isNumber() || !args[2].isNumber() || !args[3].isNumber())
				{
					jsi::detail::throwJSError(rt, "React-Native-Clusterer: getTile expects 3 numbers as arguments");
					return jsi::Value();
				}

				return cluster_getLeaves(rt, args[0].asString(rt).utf8(rt), (int)args[1].asNumber(), (int)args[2].asNumber(), (int)args[3].asNumber());
				});

		auto getClusterExpansionZoom = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "getClusterExpansionZoom"), 2, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
																			 {
				if (count != 2)
				{
					 jsi::detail::throwJSError(rt, "React-Native-Clusterer: getClusterExpansionZoom expects 2 arguments");
					 return jsi::Value();
				}
				if (!args[0].isString() || !args[1].isNumber())
				{
					 jsi::detail::throwJSError(rt, "React-Native-Clusterer: getClusterExpansionZoom expects string and a number as arguments");
					 return jsi::Value();
				}

				return cluster_getClusterExpansionZoom(args[0].asString(rt).utf8(rt), (int)args[1].asNumber());
				});

		auto destroyCluster = jsi::Function::createFromHostFunction(jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "destroyCluster"), 1, [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args, size_t count) -> jsi::Value
																			 {
				cluster_destroyCluster(args[0].asString(rt).utf8(rt));
				return jsi::Value();
				});

		jsi::Object module = jsi::Object(jsiRuntime);
		module.setProperty(jsiRuntime, "init", move(init));
		module.setProperty(jsiRuntime, "getTile", move(getTile));
		module.setProperty(jsiRuntime, "getClusters", move(getClusters));
		module.setProperty(jsiRuntime, "getChildren", move(getChildren));
		module.setProperty(jsiRuntime, "getLeaves", move(getLeaves));
		module.setProperty(jsiRuntime, "getClusterExpansionZoom", move(getClusterExpansionZoom));
		module.setProperty(jsiRuntime, "destroyCluster", move(destroyCluster));

		jsiRuntime.global().setProperty(jsiRuntime, "clustererModule", move(module));
	}

	void cleanup() {
		cluster_cleanup();
	}
}
