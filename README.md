# Generic B-Tree on Hard Disk

> A generic C++ B-Tree that keeps the **index in RAM** while storing **records on disk**.

The project explores a simple storage design:

```text
             RAM
      ┌─────────────────┐
      │   B-Tree        │
      │ Key → Offset    │
      └────────┬────────┘
               │
            seek()
               ▼
             Disk
      ┌─────────────────┐
      │    data.dat     │
      │    Records      │
      └─────────────────┘
```

Instead of storing complete records inside the tree, each key stores an offset pointing to its record in `data.dat`. This keeps the in-memory index smaller, especially when records are large.

## Highlights

* **Generic B-Tree** implemented with C++ templates.
* **Disk-backed records** using file offsets.
* **Bidirectional iterator** for forward and reverse traversal.
* Separate layers for **B-Tree logic** and **disk storage**.

The core structure is:

```cpp
__Btree<T, MAX>
```

while the disk-backed interface is:

```cpp
Btree<KeyType, ValueType, BTreeOrder>
```

Each stored key is associated with a `KeyObj` containing the key and its disk offset.

## Interesting Implementation Detail

The iterator maintains the traversal path using a stack, allowing both:

```cpp
++it
--it
```

for bidirectional traversal.

## Limitations

The current implementation does not support duplicate keys, key deletion, or general serialization of complex C++ objects.

## Possible Extensions

Persistent B-Tree nodes, record updates, and more robust serialization would be natural next steps.

**Generic Programming — 10CS368**

**Abdus Salam Khazi · Akshay Mallya · Abhishek Patil**
