def simulate_replacement(ref_string, num_frames, algorithm="FIFO"):
    frames = []
    faults = 0
    print(f"=== {algorithm} ===")
    
    for ref in ref_string:
        if ref not in frames:
            faults += 1
            evicted = None
            if len(frames) == num_frames:
                evicted = frames.pop(0)  # Both FIFO and LRU pop from index 0
            frames.append(ref)
            
            display = frames + ["_"] * (num_frames - len(frames))
            evict_str = f"  (evicted {evicted})" if evicted is not None else ""
            print(f"Ref {ref} | FAULT | frames: {display}{evict_str}")
        else:
            if algorithm == "LRU":
                frames.remove(ref)
                frames.append(ref) # Refresh recency by moving to the end
                
            display = frames + ["_"] * (num_frames - len(frames))
            print(f"Ref {ref} | HIT   | frames: {display}")
            
    print(f"Total page faults ({algorithm}): {faults}\n")
    return faults

if __name__ == "__main__":
    my_string = [6, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3]
    full_string = [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1]
    frames = 3

    print("--- Running My String ---")
    simulate_replacement(my_string, frames, "FIFO")
    simulate_replacement(my_string, frames, "LRU")
    
    print("--- Running Full Lecture String ---")
    simulate_replacement(full_string, frames, "FIFO")
    simulate_replacement(full_string, frames, "LRU")