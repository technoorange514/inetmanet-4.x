//
// Copyright (C) 1992-2004 OpenSim Ltd.
// Copyright (C) 2014 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/common/INETDefs.h"

namespace inet {

namespace utils {

cNEDValue nedf_hasModule(cComponent *context, cNEDValue argv[], int argc)
{
    if (argv[0].getType() != cNEDValue::STRING)
        throw cRuntimeError("hasModule(): string arguments expected");
    const char *name = argv[0].stringValue();
#if OMNETPP_BUILDNUM < 2000
    cRegistrationList *types = omnetpp::internal::componentTypes.getInstance();
    cComponentType *c;
    c = dynamic_cast<cComponentType *>(types->lookup(name)); // by qualified name
    if (c && c->isAvailable())
         return true;
    c = dynamic_cast<cComponentType *>(types->find(name)); // by simple name
    if (c && c->isAvailable())
         return true;
    return false;
#else
    cModuleType *type = cModuleType::find(name); // by qualified name
    if (type && type->isAvailable())
        return true;
    auto types = cModuleType::findAll(name); // by simple name
    for (cModuleType *type : types)
        if (type && type->isAvailable())
        return true;
    return false;
#endif
}

Define_NED_Function2(nedf_hasModule,
        "bool hasModule(string nedTypeName)",
        "string",
        "Returns true if the given NED type exists"
        );

cNEDValue nedf_haveClass(cComponent *context, cNEDValue argv[], int argc)
{
    return cObjectFactory::find(argv[0].stringValue()) != nullptr;
}

Define_NED_Function2(nedf_haveClass,
        "bool haveClass(string className)",
        "string",
        "Returns true if the given C++ class exists"
        );

cNEDValue nedf_moduleListByPath(cComponent *context, cNEDValue argv[], int argc)
{
    std::string modulenames;
    cTopology topo;
    std::vector<std::string> paths;
    for (int i = 0; i < argc; i++)
        paths.push_back(argv[i].stdstringValue());

    topo.extractByModulePath(paths);

    for (int i = 0; i < topo.getNumNodes(); i++) {
        std::string path = topo.getNode(i)->getModule()->getFullPath();
        if (modulenames.length() > 0)
            modulenames = modulenames + " " + path;
        else
            modulenames = path;
    }
    return modulenames;
}

Define_NED_Function2(nedf_moduleListByPath,
        "string moduleListByPath(string modulePath,...)",
        "string",
        "Returns a space-separated list of the modules at the given path(s). "
        "See cTopology::extractByModulePath()."
        );

cNEDValue nedf_moduleListByNedType(cComponent *context, cNEDValue argv[], int argc)
{
    std::string modulenames;
    cTopology topo;
    std::vector<std::string> paths;
    for (int i = 0; i < argc; i++)
        paths.push_back(argv[i].stdstringValue());

    topo.extractByNedTypeName(paths);

    for (int i = 0; i < topo.getNumNodes(); i++) {
        std::string path = topo.getNode(i)->getModule()->getFullPath();
        if (modulenames.length() > 0)
            modulenames = modulenames + " " + path;
        else
            modulenames = path;
    }
    return modulenames;
}

Define_NED_Function2(nedf_moduleListByNedType,
        "string moduleListByNedType(string nedTypeName,...)",
        "string",
        "Returns a space-separated list of the modules with the given NED type(s). "
        "See cTopology::extractByNedTypeName()."
        );

cNEDValue nedf_select(cComponent *context, cNEDValue argv[], int argc)
{
    long index = argv[0];
    if (index < 0)
        throw cRuntimeError("select(): negative index %ld", index);
    if (index >= argc - 1)
        throw cRuntimeError("select(): index=%ld is too large, max value is %d", index, argc - 1);
    return argv[index + 1];
}

Define_NED_Function2(nedf_select,
        "any select(int index, ...)",
        "misc",
        "Returns the <index>th item from the rest of the argument list; numbering starts from 0."
        );

cNEDValue nedf_absPath(cComponent *context, cNEDValue argv[], int argc)
{
    if (argc != 1)
        throw cRuntimeError("absPath(): must be one argument instead of %d argument(s)", argc);
    if (context == nullptr)
        throw cRuntimeError("absPath(): context component required");
    const char *path = argv[0].stringValue();
    switch (*path) {
        case '.':
            return context->getFullPath() + path;

        case '^':
            return context->getFullPath() + '.' + path;

        default:
            return argv[0];
    }
}

Define_NED_Function2(nedf_absPath,
        "string absPath(string modulePath)",
        "string",
        "Returns absolute path of given module"
        );

cNEDValue nedf_firstAvailableOrEmpty(cComponent *context, cNEDValue argv[], int argc)
{
#if OMNETPP_BUILDNUM < 2000
    cRegistrationList *types = omnetpp::internal::componentTypes.getInstance();
    for (int i = 0; i < argc; i++) {
        if (argv[i].getType() != cNEDValue::STRING)
            throw cRuntimeError("firstAvailable(): string arguments expected");
        const char *name = argv[i].stringValue();
        cComponentType *c;
        c = dynamic_cast<cComponentType *>(types->lookup(name)); // by qualified name
        if (c && c->isAvailable())
            return argv[i];
        c = dynamic_cast<cComponentType *>(types->find(name)); // by simple name
        if (c && c->isAvailable())
            return argv[i];
    }
    return "";
#else
    // note: diff is due to cComponentType::findAll() being absent in OMNeT++ 6.0 and earlier
    for (int i = 0; i < argc; i++) {
        if (argv[i].getType() != cNEDValue::STRING)
            throw cRuntimeError("firstAvailable(): string arguments expected");
        const char *name = argv[i].stringValue();
        cComponentType *type = cComponentType::find(name); // by qualified name
        if (type && type->isAvailable())
            return argv[i];
        auto types = cComponentType::findAll(name); // by simple name
        for (cComponentType *type : types)
            if (type && type->isAvailable())
                return argv[i];
    }
    return "";
#endif
}

Define_NED_Function2(nedf_firstAvailableOrEmpty,
    "string firstAvailableOrEmpty(...)",
    "misc",
    "Accepts any number of strings, interprets them as NED type names "
    "(qualified or unqualified), and returns the first one that exists and "
    "its C++ implementation class is also available. Returns empty string if "
    "none of the types are available.");

cNEDValue nedf_nanToZero(cComponent *context, cNEDValue argv[], int argc)
{
    double x = argv[0].doubleValueRaw();
    const char *unit = argv[0].getUnit();
    return std::isnan(x) ? cNEDValue(0.0, unit) : argv[0];
}

Define_NED_Function2(nedf_nanToZero,
        "quantity nanToZero(quantity x)",
        "math",
        "Returns the argument if it is not NaN, otherwise returns 0."
        );

static cNedValue nedf_intWithUnit(cComponent *context, cNedValue argv[], int argc)
{
    switch (argv[0].getType()) {
        case cNedValue::BOOL:
            return (intval_t)(argv[0].boolValue() ? 1 : 0);
        case cNedValue::INT:
            return argv[0];
        case cNedValue::DOUBLE:
            return cNedValue(checked_int_cast<intval_t>(floor(argv[0].doubleValueRaw())), argv[0].getUnit());
        case cNedValue::STRING:
            throw cRuntimeError("intWithUnit(): Cannot convert string to int");
        case cNedValue::OBJECT:
            throw cRuntimeError("intWithUnit(): Cannot convert cObject to int");
        default:
            throw cRuntimeError("Internal error: Invalid cNedValue type");
    }
}

Define_NED_Function2(nedf_intWithUnit,
    "intquantity intWithUnit(any x)",
    "conversion",
    "Converts x to an integer (C++ long), and returns the result. A boolean argument becomes 0 or 1; a double is converted using floor(); a string or an XML argument causes an error.");

cNedValue nedf_xmlattr(cComponent *context, cNedValue argv[], int argc)
{
    if (argv[0].getType() != cNedValue::OBJECT)
        throw cRuntimeError("xmlattr(): xmlNode argument must be an xml node");
    if (argv[1].getType() != cNEDValue::STRING)
        throw cRuntimeError("xmlattr(): attributeName argument must be a string");

    cXMLElement *node = argv[0].xmlValue();
    const char *attr = node->getAttribute(argv[1].stdstringValue().c_str());
    if (attr != nullptr)
        return cNedValue(attr);
    if (argc < 3)
        throw cRuntimeError("Attribute '%s' not found in xml '%s'", argv[1].stdstringValue().c_str(), argv[0].stdstringValue().c_str());
    return argv[2];
}

Define_NED_Function2(nedf_xmlattr,
        "string xmlattr(xml xmlNode, string attributeName, string defaultValue?)",
        "xml",
        "Returns the value of the specified XML attribute of xmlNode. "
        "It returns the defaultValue (or throws an error) if the attribute does not exists."
        )


cNEDValue nedf_selectWithRandomDistribution(cComponent *context, cNEDValue argv[], int argc)
{
    if (argc != 2)
        throw cRuntimeError("selectWithRandomDistribution(): Attribute list is not 2 Arguments : %i", argc);

    if (argv[0].getType() != cNEDValue::STRING)
        throw cRuntimeError("selectWithRandomDistribution(): string arguments expected, argument 1");

    if (argv[1].getType() != cNEDValue::STRING)
        throw cRuntimeError("selectWithRandomDistribution(): string arguments expected, argument 2");

    if (context == nullptr)
        throw cRuntimeError("selectWithRandomDistribution(): context component required");

    cStringTokenizer tokenizer1(argv[0]);
    cStringTokenizer tokenizer2(argv[1]);

    auto list1 = tokenizer1.asVector();
    auto list2 = tokenizer2.asDoubleVector();

    if (list1.size() != list2.size())
        throw cRuntimeError("selectWithRandomDistribution(): argument 1 and 2 has different numbers of elements");

    double val = 0;
    for (auto elem : list2)
        val += elem;

    if (std::abs(val-1.0) > 0.00001)
        throw cRuntimeError("selectWithRandomDistribution(): The probability list doesn't add 1 %f", val);

    // generate probability distribution list
    val = 0;
    for (auto &elem : list2) {
        double temp = elem;
        elem += val;
        val += temp;
    }

    double p = context->uniform(0, 1.0);

    for (unsigned int i = 0; i < list2.size(); i++) {
        if (p < list2[i]) {
            return list1[i];
        }
    }
    return list1.back();
}

Define_NED_Function2(nedf_selectWithRandomDistribution,
        "string selectWithRandomDistribution(string list, string probabilityList)",
        "string",
        "return an element of the first list using with the probability distribution of the second list"
        );

cNEDValue nedf_findArrayObjectElement(cComponent *context, cNEDValue argv[], int argc)
{
    cValueArray *array = check_and_cast<cValueArray *>(argv[0].objectValue());
    for (int index = 0; index < array->size(); index++) {
        cValueMap *map = check_and_cast<cValueMap *>(array->get(index).objectValue());
        for (int i = 1; i < argc; i += 2)
            if (map->get(argv[i].stringValue()).intValue() != argv[i + 1].intValue())
                goto next;
        return map;
        next:;
    }
    return cNEDValue((cObject *)nullptr);
}

Define_NED_Function2(nedf_findArrayObjectElement,
        "bool findArrayObjectElement(any array, string key, any value, ...)",
        "misc",
        "Returns the first object from the array that matches the given set of key-value pairs"
        );

cValue nedf_getId(cComponent *context, cValue argv[], int argc)
{
    if (context == nullptr)
        throw cRuntimeError("absPath(): context component required");
    return context->getId();
}

Define_NED_Function2(nedf_getId,
    "int getId()",
    "ned",
    "Returns the id of the module or channel in context.")

} // namespace utils

} // namespace inet

