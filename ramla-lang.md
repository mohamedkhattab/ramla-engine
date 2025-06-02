## Example Syntax
```lua
local shouldShowSomeItem = false
local element = Button(
    id="my-container"
    if-clicked=${
        return Menu(
            id="context-menu"
            if-hovered={
                return MenuItemsList(
                    id="menu-items-list"
                    content={
                        MenuItem(
                            id="open-item"
                            lable="open"
                            if-clicked={
                                shouldShowSomeItem = true
                                return FilePickerDialog(
                                    id="file-picker-dialog"
                                    content={}
                                )
                            }
                        )
                    }
                    if-[shouldShowSomeItem]={
                        return MenuItem(
                            id="conditionally-rendered"
                            lable="will render conditionally on next frame"
                        )
                    }
                )
            }
        )
    }
)
```