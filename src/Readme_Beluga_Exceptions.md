
This code is copied from //https://stackoverflow.com/questions/348833/how-to-know-the-exact-line-of-code-where-an-exception-has-been-caused/348862#348862

It throws an exception which includes the filename and line of code it occurs at.
This is useful for debugging as a more useful alternative to assert(False) in places 
where something has gone wrong.


Example usage:

void f() {
    throw_exception("Oh no!");
}

int main() {
    try {
        f();
    }
    catch (const std::runtime_error &ex) {
        Serial.println( ex.what() );
    }
}