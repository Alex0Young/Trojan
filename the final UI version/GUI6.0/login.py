import tkinter as tk
import pickle
import tkinter.messagebox
import mainpage

window = tk.Tk();
window.title('Turkey Trojans')
window.geometry('450x400')

canvas = tk.Canvas(window, height=200, width=500)#创建画布
image_file = tk.PhotoImage(file='welcome.gif')#加载图片文件
image = canvas.create_image(0,0, anchor='nw', image=image_file)#将图片置于画布上
canvas.pack(side='top')#放置画布（为上端）

#label
tk.Label(window, text='User name: ').place(x=50, y= 150)#label for username
tk.Label(window, text='Password: ').place(x=50, y= 190)#label for password

#entry
var_usr_name = tk.StringVar()
var_usr_name.set('2016211322')#提示

entry_usr_name = tk.Entry(window, textvariable=var_usr_name)
entry_usr_name.place(x=160, y=150)

var_usr_pwd = tk.StringVar()
entry_usr_pwd = tk.Entry(window, textvariable=var_usr_pwd, show='*')
entry_usr_pwd.place(x=160, y=190)

#function
def usr_login():
	usr_name = var_usr_name.get()
	usr_pwd = var_usr_pwd.get()

	##这里设置异常捕获，当我们第一次访问用户信息文件时是不存在的，所以这里设置异常捕获。
	##中间的两行就是我们的匹配，即程序将输入的信息和文件中的信息匹配。
	try:
	    with open('usrs_info.pickle', 'rb') as usr_file:
	        usrs_info = pickle.load(usr_file)
	except FileNotFoundError:
	 #没有usr_file，创建一个，写入admin-admin
	    with open('usrs_info.pickle', 'wb') as usr_file:
	        usrs_info = {'admin': 'admin'}
	        pickle.dump(usrs_info, usr_file)

	if usr_name in usrs_info:
	    if usr_pwd == usrs_info[usr_name]:
	        login_yes = tk.messagebox.showinfo(title='Welcome', message='How are you? ' + usr_name)
	        if login_yes:
	        	window.destroy()
	        	mainpage()
	    ##如果用户名匹配成功，而密码输入错误，则会弹出'Error, your password is wrong, try again.'
	    else:
	        tk.messagebox.showerror(message='Error, your password is wrong, try again.')
	else:   # 如果发现用户名不存在
	    is_sign_up = tk.messagebox.askyesno('Welcome','You have not sign up yet. Sign up today?')
	    # 提示需不需要注册新用户
	    if is_sign_up:
	        usr_sign_up()

def usr_sign_up():
	def sign_to_check():
		np = new_pwd.get()
		npf = new_pwd_confirm.get()
		nn = new_name.get()

		with open('usrs_info.pickle','rb') as usr_file:
			exist_usr_info = pickle.load(usr_file)
		if np!= npf:
			tk.messagebox.showerror('Error','Password and confirm password must be the same!')#两次密码不一样
		elif nn in exist_usr_info:
			tk.messagebox.showerror('Error','The user has already signed up!') #用户名已经注册过
		else:
			exist_usr_info[nn] = np#正确则添加入
			with open('usrs_info.pickle','wb') as usr_file:
				pickle.dump(exist_usr_info,usr_file)
			tk.messagebox.showinfo('Welcome','You have successfully signed up!')
			window_sign_up.destroy()


	window_sign_up = tk.Toplevel(window)
	window_sign_up.geometry('300x250')
	window_sign_up.title('Sign up window')
	new_name = tk.StringVar()

	new_name.set('example=2016211322')
	tk.Label(window_sign_up,text='User name:').place(x=10,y=10)
	entry_new_name = tk.Entry(window_sign_up,textvariable=new_name)
	entry_new_name.place(x=150,y=10)

	new_pwd = tk.StringVar()
	tk.Label(window_sign_up,text='Password: ').place(x=10,y=50)
	entry_usr_pwd = tk.Entry(window_sign_up,textvariable=new_pwd,show='*')
	entry_usr_pwd.place(x=150,y=50)

	new_pwd_confirm = tk.StringVar()
	tk.Label(window_sign_up, text='Confirm password: ').place(x=10,y =90)
	entry_usr_pwd_confirm = tk.Entry(window_sign_up, textvariable = new_pwd_confirm,show='*')
	entry_usr_pwd_confirm.place(x=150, y=90)
	btn_comfirm_sign_up = tk.Button(window_sign_up , text='sign up', command=sign_to_check)
	btn_comfirm_sign_up.place(x=150,y=130)

#button
btn_login = tk.Button(window, text='Login', command=usr_login)
btn_login.place(x=170, y=230)
btn_sign_up = tk.Button(window, text='Sign up', command=usr_sign_up)
btn_sign_up.place(x=270, y=230)


window.mainloop()