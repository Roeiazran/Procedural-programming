
def is_obj_string_valid(string_check):
    """Validate string to be a dictionary.

    Args:
        string_check (string): the string to check
    
    Returns:
        int: 1 if the string is valid 0 otherwise
    """
    # check for curly brackets surrounds the object
    if string_check[-1] != '}' or string_check[0] != '{':
        return 0
    # checks for only one currely brackets appered in the string
    if string_check.count("{") > 1 or string_check.count("}") > 1:
        return 0
    # check the right amount of perticular characters
    if string_check.count(":") != (string_check.count(",") + 1):
        return 0
    return 1


def is_obj_key_value_valid(key, vl):
    """Validate the key and value.

    Args:
        key (string): the key
        vl (string): the value
    
    Returns:
        int: 1 if the values are valid 0 otherwise
    """
    # initalize keys to check
    check_keys = ["tries", "word_length"]
    # checks for empty keys
    if not len(key):
        return 0
    # check for spaces in the keys
    if " " in key:
        return 0
    # check if the key need to be further checked
    if key in check_keys:
        # checks the keys according to the game requests
        if not vl.isdigit() or int(vl) <= 1:
            return 0
    return 1


def string_to_obj(dict_string):
    """Turn string to dinctionary.
    
    Args:
        dict_string (string): the string to convert
    
    Returns:
        dictionary: new settings dictionary
    """
    settings = {}
    # removes the curly brackets and split to a list
    dict_string = dict_string.strip("{}").split(",")
    # run for every pair of key value
    for pair in dict_string:
        # split to another list
        key, vl = pair.split(":")
        # remove unwanted spaces
        key = key.strip()
        vl = vl.strip()
        # validate the key and value to match the game requires
        if not is_obj_key_value_valid(key, vl) or key in settings:
            return print("Invalid settings")
        # if the value is only digits convert it to int
        if vl.isdigit():
            vl = int(vl)
        # updates settings
        settings[key] = vl

    return settings

def change_settings(old_settings):
    """Update the settings.

    Args:
        old_settings (dictionary): the game settings

    Returns:
        None: returns print as exit
    """
    # pompt the user to enter the settings
    new_settings = input("Enter settings:\n")
    # validates the user input
    if not is_obj_string_valid(new_settings):
        return print("Invalid settings")
    
    # convert the string into dictionary
    result_dictionary = string_to_obj(new_settings)
    # checks for empty settings
    if result_dictionary:
        # update the settings for every new setting from user
        for key, setting in result_dictionary.items():
            old_settings[key] = setting
        print("Settings were updated")


def print_game_open_message(tries, word_length):
    """Print the open message of the game.

    Args:
        tries (int): the allowed tries in the game
        word_length (int): the allowed word length of the game
    """
    print(f"Welcome to Wordle! You have {tries} tries", end=' ')
    print("to guess the word.")
    print(f"The word is {word_length} letters long.")


def update_feedback(feedback_obj, guess, is_first):
    """Update the feedback.

    Args:
        feedback_obj (dictionary): feedback details need to be updated
        guess (string): the user guessed word
        is_first (boolean): indicate num of calls to this function
    """
    word_to_guess = feedback_obj['word_to_guess']
    feedback = feedback_obj['feedback']
    length = len(guess)
    # run for every letter on guess
    for index in range(0, length):
        # if it the first time calling
        if is_first:
            # checks if the latter is in the right place
            if word_to_guess[index] == guess[index]:
                # update the feedback string to now have the letter
                feedback += guess[index]
                # update the word_to_guess to avoid duplicate letters
                word_to_guess = " ".join(
                    [word_to_guess[:index], word_to_guess[index + 1:]],
                )
            # if its not the same letter
            else:
                feedback = "".join([feedback, "-"])
        # if it is the second call to this function
        else:
            # check if the the current letter exists the guess
            if word_to_guess.find(guess[index]) != -1:
                # avoid overwriting when there is more then one of the same letter
                if feedback[index] == '-':
                    feedback = "+".join(
                        [feedback[0:index], feedback[index + 1:]],
                    )
    # update to avoid lost of data in the second call
    feedback_obj['feedback'] = feedback
    feedback_obj['word_to_guess'] = word_to_guess
    

def append_guess_feedback(word_to_guess, guess, game_object):
    """Append the user guess to the game_object.

    Args:
        word_to_guess (string): the word to be guessed on the game
        guess (string): the user guess
        game_object (dictionary): the game details
    """
    feedback = ""
    is_first = True
    # checks if the current guess is the right one
    if guess == word_to_guess:
        # update the game_object
        game_object['feedbacks'].append(guess)
    else:
        # init the feedback details object
        feedback_obj = {"word_to_guess": word_to_guess, "feedback": feedback}
        # update it to match the exercise requests
        update_feedback(feedback_obj, guess, is_first)
        update_feedback(feedback_obj, guess, not is_first)
        # update the game_object
        game_object['feedbacks'].append(feedback_obj['feedback'])
    # print to the user
    print(game_object['feedbacks'][-1])


def check_the_guess(game_object, word_to_guess, words, word_length):
    """Check the user guess.

    Args:
        game_object (dictionary): dictionary holds the games data
        word_to_guess (string): the word to guess in the game
        words (list): the list of all words
        word_length (int): the allowed length to guess
    """
    # extract the current guess
    guess = game_object['guess']
    # checks for win
    if guess == word_to_guess:
        # update game status and the tries
        game_object['status'] = 0
        game_object['player_tries'] += 1
        append_guess_feedback(word_to_guess, guess, game_object)
    # words not match case, check the length
    elif len(guess) != word_length or guess not in words:
        print("Invalid guess")
    # right length but not the right word case
    else:
        game_object['player_tries'] += 1
        append_guess_feedback(word_to_guess, guess, game_object)


def print_array(arr):
    """Print a given array.

    Args:
        arr (list): the array to prints
    """
    for st in arr:
        print(st)


def check_for_win(game_object, word_to_guess):
    """Check for winning the game.

    the function will print the winning/lossing message
    and call to print the game feedbacks made during the game

    Args:
        game_object (dictionary): the current game details
        word_to_guess (string): the word to guess in the game
    
    Returns:
        int: 1 if there is a win 0 other wise
    """
    # check the status that indicate the game status
    if game_object['status'] == 0:
        print("You win!\nGame over!")
        print_array(game_object['feedbacks'])
        return game_object['player_tries']
    else:
        print("You lost! The word was", word_to_guess)
        print("Game over!")
        print_array(game_object['feedbacks'])
    return 0

    
def play_game(settings, word_to_guess, words):
    """Playes the game.

    the function will prompt the user to guess the word and checks
    the user entered guess

    Args:
        settings (dictionary): the global settings
        word_to_guess (string): the word to guess
        words (dictionary): the global words

    Returns:
        int: the player tries until the win or else 0
    """
    # extract the tries and initalize new game_object
    tries = settings['tries']
    game_object = {'status': 1, 'player_tries': 0, 'guess': "", 'feedbacks': []}
    print_game_open_message(settings['tries'], settings['word_length'])
    # run all while tries not exceeded the max tries and no win
    while game_object["player_tries"] < tries and game_object['status']:
        game_object['guess'] = input("Guess a word:\n")
        check_the_guess(game_object, word_to_guess, words, settings['word_length'])

    return check_for_win(game_object, word_to_guess)


def validate_user_input(word_to_guess, words, word_length):
    """Validate the user input.

    the function will check the word enterd by user

    Args:
        word_to_guess (string): game guess
        words (dictionary): words
        word_length (int): required length a word

    Returns:
        int: contain the result of the validation
    """
    # validate if the word is exists
    if word_to_guess not in words:
        print("That's not a word!")
        return 0
    # validate the word length
    elif len(word_to_guess) != word_length:
        print("That word is in the wrong length!")
        return 0
    return 1


def store_player_details(players, settings, words):
    """Store the player details.

    the function will start the game after storing the user details

    Args:
        players (dictionary): the players global object
        settings (dictionary): the settings global object
        words (list): the global list of words
    """
    name = input("Enter player's name:\n")
    word_to_guess = input("Enter a word:\n")
    if validate_user_input(word_to_guess, words, settings['word_length']):
        # creates new player if non exists
        if name not in players:
            players[name] = {
                'games_played': 1,
                'wins': 0,
                'tries': 0,
            }
        # increment the player games_played attribute
        else:
            players[name]['games_played'] += 1
        # start the game and retrive the tries
        win_tries = play_game(settings, word_to_guess, words)
        # if there was a win update user attributes
        if win_tries:
            players[name]['tries'] += win_tries
            players[name]['wins'] += 1


def sort_keys_alphabetic(to_sort):
    """Sort a given dictionary.

    the function will sort the dictionary by alphabetic
    order of the keys

    Args:
        to_sort (dictionary): the dictionary to sort
    
    Returns:
        dictionary: sorted dictionary for to_sort
    """
    # first get the keys and sort them
    sorted_keys = list(to_sort.keys())
    sorted_keys = sorted(sorted_keys)
    # create object using the ordered keys
    return {index: to_sort[index] for index in sorted_keys}


def sort_by_win_rate(players):
    """Sort the players.

    the function will sort the players dictionary by their win rate

    Args:
        players (dictionary): players data structure

    Returns:
        list: contains tuples in the form (name, win rate)
    """
    # init the list to sort
    sorted_players = []
    # make the list to sort
    for player in players.keys():
        wins = players[player]['wins']
        games_played = players[player]['games_played']
        # calculate the win rates
        win_rate = round((wins / games_played) * 100, 2)
        # append the name and win rate to be sorted
        sorted_players.append((player, win_rate))
    # sorts the list by the win rate as the first
    # criteria and by the name as second
    return sorted(sorted_players, key=lambda col: (-col[1], col[0]))


def print_settings(settings):
    """Print the settings.
     
    the function prints the setting sorted by alphabetic order.
    
    Args:
        settings (dictionary): the game settings
    """
    # runs through sorted settings
    sorted_settings = sort_keys_alphabetic(settings)
    for key in sorted_settings.keys():
        print(f"{key}: {sorted_settings[key]}")


def print_win_table(players):
    """Print the win table.

    Args:
        players (dictionary): players data structure
    """
    # runs for every player in the sorted list
    print("Scoreboard:")
    for (name, win_rate) in sort_by_win_rate(players):

        # calculate avarage tries
        tries = players[name]['tries']
        wins = players[name]['wins']
        avg_tries = 0 if wins == 0 else round(tries / wins, 2)
        # format the output string
        st = "{0}: {1} games, {2:.2f}% win rate,".format(
            name,
            players[name]['games_played'],
            win_rate,
        )
        # format the output string dymaicaly
        if wins == 0:
            st = " ".join([st, "NaN:-) average tries"])
        else:
            st = " ".join([st, f"{avg_tries:.2f} average tries"])

        print(st)


def handle_choice(choice, settings, players, words):
    """Handle user choice.

    Args:
        choice (int): the users choice
        settings (dictionary): the game initial settings
        players (dictionary): the players initial data structure
        words (list): a list of all the words
    """
    if choice == 1:
        change_settings(settings)
    elif choice == 2:
        store_player_details(players, settings, words)
    elif choice == 3:
        print_settings(settings)
    elif choice == 4:
        print_win_table(players)
    else:
        print("Invalid option")


def print_menu():
    """Print the menu."""
    print("Choose an option:")
    print("0. Exit")
    print("1. Update settings")
    print("2. Play")
    print("3. View settings")
    print("4. Scoreboard")
      

def main():
    """Start the program.
     
    the function handles the user choice and init global variables.
    """
    settings = {"tries": 6, "word_length": 5, "file_path": "words.txt"}

    # init players data structure to store all the players details
    players = {}
    while True:
        file = open(settings['file_path'], "r")
        words = file.read().splitlines()
        # prints the menu
        print_menu()
        choice = input()
        # avoids characters input
        if not choice.isdigit():
            continue
        choice = int(choice)
        # breaks if choice is zero
        if choice == 0:
            file.close()
            break
        # handles the user input choice
        handle_choice(choice, settings, players, words)

   
if __name__ == "__main__":

    main()