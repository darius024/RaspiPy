def update_count(count):
{    if (count >= 10):
        {return count}
    else:
        {return update_count(count + 1)}
}
counter = 0
counter = update_count(counter)

