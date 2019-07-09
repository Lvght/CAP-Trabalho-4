function filtro() {
    var e=event.keyCode;

    if (event.shiftKey == false){
        if (e==8 || e==32 || e==35 || e==36 || e==46 || (e>=48 && e<=57) || (e>=65 && e<=90) || (e>=96 && e<=105)){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}