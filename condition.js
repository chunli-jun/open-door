
const devicesId = "562127032";
const api_key = "gWYB0GA6bQ9vnbAPqMp0BRVgdBI=";
Page({
  data: {
   
  },

  formSubmit: function (e) {
    var reply = e.detail.value.reply;
    this.setData({
    

       
        reply: reply,


    })
    //通过这个语句触发下面的sendCmd函数，向onenet发送数据流
    this.sendCmd(this.data.reply);

    console.log(reply)

  },

  //这个函数实现对输入框数据的储存
  searchBox: function (e) {
    const that = this;
    let reply;
    that.setData({
      reply: e.detail.value.setreply//把输入的值附到定义的变量里，进行储存

    })
    //通过这个语句触发下面的sendCmd函数，向onenet发送数据流
    this.sendCmd(this.data.reply);
  },



  //向onenet发送数据
  sendCmd: function (reply) {//要发送的数据流，注意顺序要与下面data中的对应
    var _this = this;
    wx.request({
      url: 'https://api.heclouds.com/devices/562127032/datapoints?type=3',//注意更改deviceID
      header: {
        'api-key': 'gWYB0GA6bQ9vnbAPqMp0BRVgdBI=',
      },
      method: 'POST',
      data: {

        reply
      },
      success: function (res) {
        console.log("success!!")
       _this.setData({
          reply: ''

        })

      },
      fail: function (res) {
        console.log("fail!!!")
      }
    })
  },
  bindViewTap: function () {
    wx.navigateTo({
      ukl: '../logs/logs'
    })
  },
})



