// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/builtins/builtins-utils.h"
#include "src/builtins/builtins.h"
#include "src/counters.h"
#include "src/objects-inl.h"

namespace v8 {
namespace internal {

// -----------------------------------------------------------------------------
// ES6 section 19.4 Symbol Objects

// ES6 section 19.4.1.1 Symbol ( [ description ] ) for the [[Call]] case.
BUILTIN(SymbolConstructor) {
  HandleScope scope(isolate);
  Handle<Object> description = args.atOrUndefined(isolate, 1);
  if (!description->IsUndefined(isolate)) {
    ASSIGN_RETURN_FAILURE_ON_EXCEPTION(isolate, description,
                                       Object::ToString(isolate, description));
  }
  RETURN_RESULT_OR_FAILURE(isolate, isolate->factory()->NewSymbol(description));
}

// ES6 section 19.4.1.1 Symbol ( [ description ] ) for the [[Construct]] case.
BUILTIN(SymbolConstructor_ConstructStub) {
  HandleScope scope(isolate);
  THROW_NEW_ERROR_RETURN_FAILURE(
      isolate, NewTypeError(MessageTemplate::kNotConstructor,
                            isolate->factory()->Symbol_string()));
}

// ES6 section 19.4.2.1 Symbol.for.
BUILTIN(SymbolFor) {
  HandleScope scope(isolate);
  Handle<Object> key_obj = args.atOrUndefined(isolate, 1);
  Handle<String> key;
  ASSIGN_RETURN_FAILURE_ON_EXCEPTION(isolate, key,
                                     Object::ToString(isolate, key_obj));
  return *isolate->SymbolFor(Heap::kPublicSymbolTableRootIndex, key, false);
}

// ES6 section 19.4.2.5 Symbol.keyFor.
BUILTIN(SymbolKeyFor) {
  HandleScope scope(isolate);
  Handle<Object> obj = args.atOrUndefined(isolate, 1);
  if (!obj->IsSymbol()) {
    THROW_NEW_ERROR_RETURN_FAILURE(
        isolate, NewTypeError(MessageTemplate::kSymbolKeyFor, obj));
  }
  Handle<Symbol> symbol = Handle<Symbol>::cast(obj);
  DisallowHeapAllocation no_gc;
  Object* result;
  if (symbol->is_public()) {
    result = symbol->name();
    DCHECK(result->IsString());
  } else {
    result = isolate->heap()->undefined_value();
  }
  DCHECK_EQ(isolate->heap()->public_symbol_table()->SlowReverseLookup(*symbol),
            result);
  return result;
}

}  // namespace internal
}  // namespace v8
