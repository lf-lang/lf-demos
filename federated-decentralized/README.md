# federated-decentralized
This demo explores one of the most sophisticated and experimental parts of Lingua Franca, **federated LF with decentralized coordination**.

## Script

1. Prerequisites: Install 
   Java 17,
   the LF VS Code extension,
   the [LF command-line tools](https://www.lf-lang.org/docs/installation#cli-tools), and
   <a href="https://libwebsockets.org">libwebsockets</a> (e.g., `brew install libwebsockets`).
2. 

## Explanations

The decentralized coordinator for federated Lingua Franca programs is an experimental technology with many subtleties. This coordinator is specified as a target property:

```
target {
  coordination: decentralized
}
...
federated reactor {
  a = new A()
  b = new B()
  ...
}
```

Each top-level reactor instance, such as `a` and `b` above, runs as a separate program called a "federate," possibly on different machines or in separate containers.

## The Role of the RTI
Like the (default) centralized coordinator, the runtime infrastrcture (RTI) orchestrates the startup (and shutdown) of the federation, but unlike the centralized coordinator, the RTI plays little role during the execution of the program. Its function is limited to:

* handling requests to shut down the federation, via `lf_request_stop()` in C or `request_stop()` in Python; and
* performing runtime clock synchronization, if this is enabled.

## STA and STAA
Each federate makes its own decisions about when to advance to a tag _g_ = (_t_, _m_) and invoke reactions at that tag.  To govern these decisions, there are two key variables that a programmer must specify:

* **STA**: The "safe to advance" offset is a physical time quantity that asserts that the federate can advance to a tag _g_ = (_t_, _m_) when its local physical clock time. _T_ satisfies _T_ >= _t_ + _STA_.
* **STAA**: The "safe to assume absent" offset is a physical time quantity that asserts that the federate can assume that an input to the federate is absent at tag _g_ = (_t_, _m_) when its local physical clock time _T_ satisfies _T_ >= _t_ + _STA_ + _STAA_.

The STA is a property of a federate and is defined as a parameter for the reactor class:

```
reactor A(STA:time = <default>) { ... }
```

The STAA is associated with one or more input ports, but it is declared on reactions to those input ports:

```
reactor A {
  input in:<type>
  reaction(in) {=
    <normal operation>
  =} STAA(<time value>) {=
    <fault handler>
  =}
}
```

Any network input port will have that STAA applied to it if it is a trigger for or is used by a reaction that declares an STAA handler.
If more than one STAA is declared for the same input port, the minimum time value will be the one in effect.  The `<fault handler>` is code that will be executed if an input had been previously assumed to be absent at a tag _g_, but then an input event with tag _g_ arrives.

Choosing suitable STAs and STAAs for a program is challenging and depends on many factors.  Collectively, STA and STAA are both referred to as **STP** (safe to process) offsets, but the distinction between the two is important and subtle.
