Page({
  data: {
    imgUrls: [
      {
        link: '/pages/logs/logs',
        url: '/images/img-5271b163544da615933a91144c105b41.jpg'
      }, {
        link: '/pages/logs1/logs1',
        url: '/images/img-c7fcecbe45c8b8dd230b67cc382fdd0a.jpg'
      }, {
        link: '/pages/logs2/logs2',
        url: '/images/img-e28a41f90218378b2c18f8df8fd5f0df.jpg'
      },
      {
        link: '/pages/logs3/logs3',
        url: '/images/taobao/-822288252.jpg'
      }
    ],
    indicatorDots: true,  //小点
    autoplay: true,  //是否自动轮播
    interval: 3000,  //间隔时间
    duration: 3000,  //滑动时间
  },
  onShareAppMessage() {
    return {
      title: 'swiper',
      path: 'page/component/pages/swiper/swiper'
    }
  },

  changeIndicatorDots() {
    this.setData({
      indicatorDots: !this.data.indicatorDots
    })
  },

  changeAutoplay() {
    this.setData({
      autoplay: !this.data.autoplay
    })
  },

  intervalChange(e) {
    this.setData({
      interval: e.detail.value
    })
  },

  durationChange(e) {
    this.setData({
      duration: e.detail.value
    })
  }
})