Fruit position picking:
    - Iterate over X_SIZE and Y_SIZE
    - Check if position (x, y) is colliding with a snake node
    - If so, skip this position
        - Otherwise, add this position to a dynamic list of positions
        - Pick a random one
        - Return it!
