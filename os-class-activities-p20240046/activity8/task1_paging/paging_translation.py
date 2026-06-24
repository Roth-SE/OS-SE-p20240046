def simulate_paging():
    P = 16
    page_table = {0: 5, 1: 2, 2: 1, 4: 7, 6: 0, 7: 4}
    addresses = [20, 100, 48, 16, 127, 64]
    
    for LA in addresses:
        page = LA // P
        offset = LA % P
        if page in page_table:
            frame = page_table[page]
            physical = frame * P + offset
            print(f"Logical {LA:3} -> page {page}, offset {offset:2} -> frame {frame} -> physical {physical}")
        else:
            print(f"Logical {LA:3} -> page {page} -> Page fault: page not in memory")

if __name__ == "__main__":
    simulate_paging()