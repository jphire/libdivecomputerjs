declare module Chai {
    interface Assertion {
        string();
        number();
        array();
    }
}
declare module 'chai-asserttype' {
    export default fn = Chai.ChaiPlugin; 
}