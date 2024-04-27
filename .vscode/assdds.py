# Функція для створення і відображення ігрової дошки
def display_board(board):
    for row in board:
        print("|".join(row))
        print("-" * 5)

def check_winner(board, player):

    for row in board:
        if all(cell == player for cell in row):
            return True

    for col in range(3):
        if all(board[row][col] == player for row in range(3)):
            return True

    if all(board[i][i] == player for i in range(3)) or all(board[i][2 - i] == player for i in range(3)):
        return True

    return False

def make_move(board, player, row, col):
    if board[row][col] == " ":
        board[row][col] = player
        return True
    else:
        return False

def tic_tac_toe():
    board = [[" " for _ in range(3)] for _ in range(3)]
    players = ["X", "O"]
    turn = 0

    while True:
        display_board(board)
        print(f"Гравець {players[turn]}, ваш хід.")

        row = int(input("Введіть номер рядка (0, 1 або 2): "))
        col = int(input("Введіть номер стовпця (0, 1 або 2): "))

        if 0 <= row <= 2 and 0 <= col <= 2:
            if make_move(board, players[turn], row, col):
                if check_winner(board, players[turn]):
                    display_board(board)
                    print(f"Гравець {players[turn]} переміг!")
                    break
                elif all(cell != " " for row in board for cell in row):
                    display_board(board)
                    print("Гра завершилася в нічию!")
                    break
                else:
                    turn = (turn + 1) % 2
            else:
                print("Ця клітинка вже зайнята! Спробуйте ще раз.")
        else:
            print("Некоректні координати! Спробуйте ще раз.")

tic_tac_toe()
