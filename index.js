
const app = getApp();
const devicesId = "562127032";
const api_key = "gWYB0GA6bQ9vnbAPqMp0BRVgdBI=";
Page({
  data: {
    motto: "" 
  
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
  /**
   * @description 页面下拉刷新事件
   */
  onPullDownRefresh: function() {
    
    this.getDatapoints().then(datapoints => {
      this.update(datapoints)
      wx.hideLoading()
    }).catch((error) => {
      wx.hideLoading()
      console.error(error)
    })
  },
    
   onLoad: function() {
    console.log(`your deviceId: ${devicesId}, apiKey: ${api_key}`);
    const timer = setInterval(() => {
      this.getDatapoints().then(datapoints => {
        this.update(datapoints)
      })
    }, 6000)
  
     this.getDatapoints().then((datapoints) => {
      wx.hideLoading()
      this.firstDraw(datapoints)
    }).catch((err) => {
      wx.hideLoading()
      console.error(err)
      clearInterval(timer) //首次渲染发生错误时禁止自动刷新
    })
  },
  getDatapoints: function() {
    wx.request({
      url: `https://api.heclouds.com/devices/${devicesId}/datastreams?datastream_ids=humanHotSensor`,
      header: {
        "content-type": "application/json",
        "api-key": api_key
      },
success: res => {
        console.log(`success`); 
        console.log(res.data.data[0]); 
        var info = res.data.data[0];
        if (info.current_value == "1.00") {
          this.setData({
            motto: "有小可爱来了~~~" 
          });
        } else {
          
          this.setData({
            motto: " 空空如也 "
          });
        }
      }

      
    });
  },
  setDatapoints: function() {
    wx.request({
      url: `https://api.heclouds.com/devices/${devicesId}/datapoints`,
      header: {
        "content-type": "application/json",
        "api-key": api_key
      },
      method: "POST",
      data: {
        datastreams: [
          {
            id: "reply",
            datapoints: [
              {
                value: this.data.reply
                
              }
            ]
          }
        ],
       

      },
      
      success: res => {
        console.log(`send successfully`);
      }
    });
    
      
  },
  
   
  

  
});
