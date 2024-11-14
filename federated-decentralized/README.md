# federated-decentralized
This demo explores one of the most sophisticated and experimental parts of [Lingua Franca](https://lf-lang.org), **[federated LF](https://www.lf-lang.org/docs/next/writing-reactors/distributed-execution/) with [decentralized coordination](https://www.lf-lang.org/docs/next/writing-reactors/distributed-execution#decentralized-coordination)**.

**STA and STAA are explained in the appendix below.**

## Sequence

1. Prerequisites: Install 
   Java 17,
   the [LF VS Code extension](https://www.lf-lang.org/docs/installation#visual-studio-code),
   the [LF command-line tools](https://www.lf-lang.org/docs/installation#cli-tools), and
   <a href="https://libwebsockets.org">libwebsockets</a> (e.g., `brew install libwebsockets`).
2. [Accounts1.lf](src/Accounts1.lf): Distributed, web interface, nondeterministic [Hewitt](https://www.sciencedirect.com/science/article/abs/pii/0004370277900339)-[Agha](https://dl.acm.org/doi/abs/10.1145/83880.84528) actors, but eventually consistent. Updates are [ACID 2.0](https://arxiv.org/abs/0909.1788). Balance state variable is a [CRDT](https://pages.lip6.fr/Marc.Shapiro/papers/RR-7687.pdf).
3. [Accounts2.lf](src/Accounts2.lf): Overdraft prevention. Non-commutative updates. No longer eventually consistent.
4. [Accounts3.lf](src/Accounts3.lf): Test of the above that attempts to show lack of eventual consistency.
3. [Accounts4.lf](src/Accounts4.lf): Time stamped updates, but no protection against near simultaneous transactions. **Discuss about clock synchronization.**
4. [Accounts5.lf](src/Accounts5.lf): A solution: [Chandy and Misra](https://ieeexplore.ieee.org/abstract/document/1702653) without null messages. This is implemented using `STA == forever` in the `AccountManager` reactors. Updates in the display are delayed arbitrarily until there is activity in the other node.
5. [Accounts6.lf](src/Accounts6.lf): Adds null messages one second apart. **Note that this does not use C&M for the input to the Server. See Discussion below.**
5. [Accounts7.lf](src/Accounts7.lf): Another solution: [PTIDES](https://ieeexplore.ieee.org/abstract/document/4155328) / [Spanner](https://www.doi.org/10.1145/2491245). This uses an STA offsets of 30 ms in the `AccountManager` reactors.  This will be consistent as long as apparent latency does not exceed 30 ms. **What happens if latencies exceed 30 ms?** The [CAL theorem](https://dl.acm.org/doi/10.1145/3609119) tells us that inconsistencies are unavoidable. We've bounded unavailability to 30 ms. If network latencies get large enough (or the network partitions), then inconsistencies cannot be prevented. See also [IT](https://spj.science.org/doi/10.34133/icomputing.0013) and [arXiv](https://arxiv.org/abs/2109.07771) papers.
6. [Accounts8.lf](src/Accounts8.lf): A hybrid solution that maintains both an available balance and a strongly consistent one (see below).
7. [Accounts9.lf](src/Accounts9.lf): An alternative hybrid solution with a different business decision that accepts withdrawls only based a strongly consistent balance while deposits and balance inquiries remain available.
8. [ImpossibleConsistency.lf](src/ImpossibleConsistency.lf): A pattern where strong consistency becomes impossible because processing offsets go to infinity (see the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119)). See details below.
9. [PossibleConsistency.lf](src/PossibleConsistency.lf): A pattern where strong consistency becomes possible again under an assumed bound on unavailability (see the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119)). Specifically, if unavailability is less than the period, then a zero processing offset for the nodes will work.  Note that this strategy will not work for the previous example even if the minimum spacing of the physical action is greater than zero. See details below.

### Handling STP Violations in Ptides/Spanner

The solution above ignores STP violations, which means that inputs to the `AccountManager` that are out of order will be handled the same way as other inputs and a warning will be issued.  This is probably not good enough for this application. At a minimum, we need eventual consistency, and the consistent value needs to account for all deposits and withdrawls that occur, even if they result in negative balances.

Shulu Li has proved (unpublished) that eventual consistency occurs if _and only if_ the merge operation is commutative, associative, and idemptotent.
Idempotency is guarateed by the framework in this case because messages are delivered exactly once.
What are the options to make the operation commutative?

#### Rollback
One technique is optimistic computation with rollback, as pioneered by [Jefferson in TimeWarp](https://dl.acm.org/doi/abs/10.1145/3916.3988).
But this has limited utility for cyber-physical systems. In this case, we can't roll back cash dispensed.

#### Hybrid Solutions

In this application, eventual inconsistencies can occur when:

* An AccountManager rejects a withdrawl because a remote deposit took longer than the STA of the AccountManager (30 ms) to arrive. The remote system may record the withdrawl as having succeeded, but the local manager has actually rejected the withdrawl and imposed an overdraft protection.
* An AccountManager dispenses cash because a remote withdrawl took longer than the STA of the AccountManager (30 ms) to arrive. In this case, two withdrawls succeed, but the local manager may record the remote withdrawl failing, and the remote manager may record the local withdrawl failing.

We correct for this in [Accounts8.lf](src/Accounts8.lf) using a form of rollback that recognizes that dispensing cash cannot be undone.
In this solution, each AccountManagerWithRecovery reports actual local transactions (cash dispensed or deposited) to a Balance reactor.
The Balance reactor has an STA set to `forever`, so its balance at any logical time will always be the actual true balance.
This is the [Chandy and Misra](https://ieeexplore.ieee.org/abstract/document/1702653) strategy, so in order to bound the lag in forming the true balance, the AccountManager sends messages at least once every 10 seconds, reporting a transaction amount of zero when there is no transaction (Chandy and Misra null messages).

The Balance reactor sends the true balance to each AccountManagerWithRecovery every 10 seconds.
This undergoes an `after` delay of 10 seconds, and hence will be reliably received if the apparently latency is less than 10 s plus the STA (30 ms).
If the message is tardy, it is ignored by the AccountManagerWithRecovery, which will just continue to use its local value of the balance, hence assuring unavailability no larger than 30 ms under all circumstances.
Note that this is business policy, not a technical requirement. Alternative policies are possible, such as taking the branch offline.

Because of the 10 s `after` delay, the AccountManagerWithRecovery will receive a true balance that is 10 seconds old.
This is a specified inconsistency, which is what makes the bounded unavailability of 30 ms possible.
Inconsistencies are (normally) transient and last no more than 10 seconds.

Every 10 seconds, AccountManagerWithRecovery updates its local copy of the balance by using the 10 s old balance it just received and replaying all inputs has received in the last 10 seconds. 

### Memory and Denial of Service Attacks

The AccountManagerWithRecovery needs to record all inputs it receives in each 10 second interval to update its local balance and correct inconsistencies.
To prevent memory problems, the number of such inputs is bounded to 10 at the SimpleWebSocketServer reactor, which enforces a minimum spacing of 1 second between messages that it forwards from the user interface.
Hence, the AccountManagerWithRecovery never needs to record more than 10 inputs.
This helps to mitigate overflow attacks, denial of service attacks, and babbling idiot faults.

### Fault Tolerance	

The Balance reactor, as designed in [Accounts8.lf](src/Accounts8.lf), will block forever if one or more of the AccountManagerWithRecovery reactors fails and stops sending null messages.
This can be prevented by using a finite STA (e.g. 10 seconds).
A local timer can be used to detect and react to this failure and take the AccountManagerWithRecovery offline.

### Comment: Input to the Web Socket Server

The `SimpleWebSocketServer` raises an interesting conundrum.
If we insist on the input being handled in tag order relative to the tags of the physical action, we have a big problem.
It does not work to set the STA because the input depends on the output. It also doesn't work to set the STAA on the input because it becomes possible for the federate to commit to tag _g_ of the physical action and then later recieve an input with tag _h_ < _g_ that is a consequence of the physical action in the _other_ `SimpleWebSocketServer`.
This will lead to an STP violation.

To handle this, we note that if we assume that the upstream reactor, `AccountManager`, processes events in tag order, then messages will be delivered to the input of the `SimpleWebSocketServer` in tag order.
There is no need for them to be handled in tag order relative to the physical action.
Hence, the implementation simply ignores STP violations and handles all inputs the same way regardless of whether they are tardy or not.  Specifically, the reaction to the input `in` looks like this:

```
  reaction(in) {=
    // Do something.
  =} STAA(0) {=
    // Do the same something.
  =}
```

Note that this pattern can also be implemented by just providing no `STAA` clause at all. But in such an implementation, warnings will be issued stating that STP violations occurred and there is no handler.

These assumptions together ensure that the web interfaces are _eventually_ consistent, even if not consistent at a specific tag.  This is adequate for this application.

For this to be correct and not undermine determinism, we must ensure that the reaction to `in` and the reaction to the physical action do not change the state of the reactor.
This is indeed the case.
But we can't separate the reactions into separate reactors (which would provide such assurance by construction) because the reactions need access to the same websocket interface to the browser.

## Impossibility

### Impossible Consistency

[ImpossibleConsistency.lf](src/ImpossibleConsistency.lf) shows a pattern where strong consistency becomes impossible because processing offsets go to infinity (see the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119)). 
In this pattern, there are no finite STA offsets that enable each Update reactor to advance its tag.
A positive value that is the same for both federates won't work because it will delay the outputs by the same amount, so waiting that amount offers no information about the remote tag.
Asymmetric values won't work either because the federate with the smaller STA will potentially prematurely advance its tag.
This situation is described in the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119) paper, where the processing offsets go to infinity.

Note that the centralized coordinator works fine for this case.
Each federate periodically notifies the RTI of the advancement of its _physical_ time without advancing its _logical_ time (something that cannot be accomplished with null messages sent based on a timer).

### Possible Consistency

In [PossibleConsistency.lf](src/PossibleConsistency.lf), strong consistency becomes possible again under an assumed bound on unavailability (see the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119)). Specifically, if unavailability is less than the period, then a zero processing offset for the nodes will work. Specifically, an STA offset of zero enables each Update reactor to advance its tag under the assumption that the period is sufficiently large that when physical time reaches a timer time, the remote input with the _previous_ timer time will have been received.
This situation is described in the [CAL theorem](https://dl.acm.org/doi/10.1145/3609119) paper.

Note that this strategy will not work for the previous example even if the minimum spacing of the physical action is greater than zero. 

Note also that the centralized coordinator does not require this assumption.


## Appendix

### Decentralized Coordination

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

### The Role of the RTI
Like the (default) centralized coordinator, the runtime infrastrcture (RTI) orchestrates the startup (and shutdown) of the federation, but unlike the centralized coordinator, the RTI plays little role during the execution of the program. Its function is limited to:

* handling requests to shut down the federation, via `lf_request_stop()` in C or `request_stop()` in Python; and
* performing runtime clock synchronization, if this is enabled.

### STA and STAA
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
